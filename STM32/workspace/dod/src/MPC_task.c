#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "button.h"
#include "led_rgb.h"
#include "light.h"
#include "potentiometer.h"
#include "encoder.h"
#include "dimmer_task.h"
#include "motor.h"
#include "reference.h"
#include "math.h"
#include "observer.h"
#include "MPC.h"
#include "MPC_task.h"
#include "os_tick.h"
#define PI 3.141592653589793
#define Ts 0.01f
#include "stepper_motor.h"
#include "kalman.h"

static int count = 0;
static osTimerId_t _MPC_task_TimerID;

static void _MPC_task_update(void *argument);

static uint8_t _is_running = 0;
static uint8_t _is_init = 0;

static float alpha0 = 0.0f; // angular accelerations alpha
static float alpha1 = 0.0f;
static float w0 = 0.0f;     // angular velocities omega
static float w1 = 0.0f;
 
static float* _mux;

void MPC_task_init(void)
{
    if (!_is_init)
    {
        // Initialise GPIO PA12 for sync pin : Active Low // no conflict - left here although redundant
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitTypeDef  GPIO_InitStructure;
        GPIO_InitStructure.Pin      = GPIO_PIN_12;
        GPIO_InitStructure.Mode     = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Pull     = GPIO_NOPULL;
        GPIO_InitStructure.Speed    = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);

        /* Initialise pin to measure calculation time using oscilliscope */ // no conflict - left here although redundant
        __HAL_RCC_GPIOC_CLK_ENABLE();
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 0);

        if (_MPC_task_TimerID == NULL)
        {
            _MPC_task_TimerID = osTimerNew(_MPC_task_update, osTimerPeriodic, (void *)0, NULL);
        }

        _is_init = 1;
    }
    // Start MPC task so robot works without serial communication.
    MPC_task_resume();
}

void MPC_task_resume(void)
{
    if (!osTimerIsRunning(_MPC_task_TimerID))
    {
        // Check if dimmer is running and stop it
        if (dimmer_task_is_running())
        {
            dimmer_task_stop();
            printf("Dimmer has stopped\n");
        }

        // Reset MPC state
        MPC_init();
        // Reset observer state - redundant
        // obs_init();
        // Reset reference
        reference_init();
        // Reset encoder
        encoder_set_count(0); //no conflict on encoder pins

        // Clear integrators and variables
        w1 = 0.0f;
        w0 = 0.0f;
        alpha1 = 0.0f;
        alpha0 = 0.0f;

        // Clear PA12 to sync dSPACE simulation
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);

        osTimerStart(_MPC_task_TimerID, 10);
        // _MPC_task_update((void*)0);
        _is_running = 1;
    }
}

void MPC_task_stop(void)
{
    if (_is_running)
    {
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);

        // Set PA12 to stop dSPACE simulation
        osTimerStop(_MPC_task_TimerID);
        MPC_free();
        _is_running = 0;
    }
}

uint8_t MPC_task_is_running(void)
{
    return _is_running;
}

void _MPC_task_update(void *argument)
{
    UNUSED(argument);

    // Warm up kalman filter
    while (count<1000)
    {
        _mux = kf_obs_states();
        float _dtheta = -_mux[0];
        float _theta = -_mux[1] - 0.090f;
        count++;
        return;
    }


    uint32_t start_ticks = osKernelGetTickCount();
    uint32_t start_count = OS_Tick_GetCount();


    // Apply previously computed control action
    w0 = w1;
    alpha0 = alpha1;
    stepper_ctrl_speed(w0);


    // Use kalman filter to estimate states
    _mux = kf_obs_states();
    float _dtheta = -_mux[0];
    float _theta = -_mux[1] - 0.090f; // Apply manual offset


    /* Reference update */
    float ref = reference_update();


    /* Assemble control model states */
    float x1[MPC_N_STATE];
    x1[0] = _dtheta;
    x1[1] = _theta;
    x1[2] = w0;


    // Use MPC control update to calculate u[k+1]
    alpha1 = MPC_run(x1, alpha0, ref);
    // printf("%f\n", alpha1);


    // integrate controller output to get velocity.
    w1 = w0 +  Ts*alpha1;
    // printf("%f\n", w1);

    uint32_t end_ticks = osKernelGetTickCount();
    uint32_t end_count = OS_Tick_GetCount();
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 0);

    // Calculate time taken to complete control sequence
    float interval = (float)OS_Tick_GetInterval();
    float start_ms = (float)start_ticks + (float)start_count/interval;
    float end_ms   = (float)end_ticks   + (float)end_count/interval;
    float timer = end_ms-start_ms;
    printf("Time [ms] = %f\n", timer);
}

void MPC_task_deinit(void)
{
    _is_init = 0;
    _is_running = 0;
}