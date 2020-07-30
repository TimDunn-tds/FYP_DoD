#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "button.h"
#include "led_rgb.h"
#include "light.h"
#include "potentiometer.h"
#include "encoder.h"
#include "dimmer_task.h"
#include "controller.h"
#include "motor.h"
#include "reference.h"
#include "math.h"
#include "controller_task.h"
#include "motor_cmd.h"
#include <stddef.h>

#include "observer.h"


#include "controller.h"
#include "stepper_motor.h"
#include "kalman.h"

#define PI 3.141592653589793f

static osTimerId_t _controller_task_TimerID;

static void _controller_task_update(void *argument);

static uint8_t _is_running = 0;
static uint8_t _is_init = 0;

static float dphi = 0.0f;
static float _dtheta = 0.0f;
static float _theta = 0.0f;
static float* _dphi;
static float* _mux;
static float* _obs;
static float dphi_dem;

static float _input[KF_N_INPUT];
static float _meas[3];
static float _output[KF_N_OUTPUT];

static int count = 0;

// static const float _Kc1 = 
// {
//     #include "Kc1.def"
// };

// static const float _Kc2 = 
// {
//     #include "Kc2.def"
// };

// static const float _Kc3 = 
// {
//     #include "Kc3.def"
// };

// static const float _N = 
// {
//     #include "N.def"
// };

void controller_task_init(void)
{
    if (!_is_init)
    {
        // Initialise GPIO PA12 for sync pin : Active Low
        // __HAL_RCC_GPIOA_CLK_ENABLE();
        // GPIO_InitTypeDef  GPIO_InitStructure;
        // GPIO_InitStructure.Pin      = GPIO_PIN_12;
        // GPIO_InitStructure.Mode     = GPIO_MODE_OUTPUT_PP;
        // GPIO_InitStructure.Pull     = GPIO_NOPULL;
        // GPIO_InitStructure.Speed    = GPIO_SPEED_FREQ_HIGH;
        // HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
        // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);

        if (_controller_task_TimerID == NULL)
        {
            _controller_task_TimerID = osTimerNew(_controller_task_update, osTimerPeriodic, (void *)0, NULL);
        }
        dphi = 0.0f;
        dphi_dem = 0.0f;
        _is_init = 1;
    }
}

void controller_task_resume(void)
{
    if (!osTimerIsRunning(_controller_task_TimerID))
    {
        // Check if dimmer is running and stop it
        if (dimmer_task_is_running())
        {
            dimmer_task_stop();
            printf("Dimmer has stopped\n");
        }

        // Reset controller state
        ctrl_init();
        // Reset reference
        reference_init();
        // Reset encoder
        encoder_set_count(0);
        // Clear phi 'integrator'
        // _phi = 0.0f;
        dphi = 0.0f;

        count = 0;

        // Clear PA12 to sync dSPACE simulation
        // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);

        osTimerStart(_controller_task_TimerID, 10);
        _is_running = 1;
    }
}

void controller_task_stop(void)
{
    if (_is_running)
    {
        // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);

        // Set PA12 to stop dSPACE simulation
        osTimerStop(_controller_task_TimerID);

        _is_running = 0;
    }
}

uint8_t controller_task_is_running(void)
{
    return _is_running;
}

void _controller_task_update(void *argument)
{
    UNUSED(argument);

    while (count<1000)
    {
        _mux = kf_obs_states();
        _dtheta = -_mux[0];
        _theta = -_mux[1] - 0.0825f;
        count++;
        printf("Warming up kalman filter\n");
        return;
    }


    /* 
    1: read imu and kalman to get theta and dtheta
    2: update the reference
    3: Update the control system
        3.1: Make sure observer is updated with the augmented state dphi
    4: Set motor speed
        4.1: Check minimum speed available
        4.2: Check max speed available
        4.2: Check torque/speed graph
    5: balance.

    */

    // get the states
    // both are given negatives to set the stm side of the robot as "forward"
    _mux = kf_obs_states();
    _dtheta = -_mux[0];
    _theta = -_mux[1] - 0.0825f;
    // make bigger to tilt toward STM side


    // update the reference
    float ref = reference_update();
    // float ref = reference_update();

    // set state
    // float state[3];
    // state[0] = _dtheta;
    // state[1] = _theta;
    // state[2] = dphi_dem;
    // ctrl_set_state(state);

    // load input
    float input[CTRL_N_INPUT];
    input[0] = ref;
    input[1] = _dtheta;
    input[2] = _theta;

    printf("dtheta: %f, theta: %f, ref: %f\n", _dtheta, _theta, ref);




    // printf("%f, %f, %f\n",input[0],input[1], input[2]);
    _dphi = ctrl_run(input);

    // printf("%f\n", dphi_dem[0]);

    // dphi_dem[0] = dphi_dem[0] + 1.0f;
    // set motor dir based on dphi demanded
    // dphi = stepper_ctralloc(dphi_dem);
    // stepper_ctrl_dir(dphi_dem);


    stepper_ctrl_speed(_dphi[0]);
    // stepper_ctrl_speed(dphi_dem);
    printf("dphi: %f\n",_dphi[0]);





}

void controller_task_deinit(void)
{
    _is_init = 0;
    _is_running = 0;
}



    // // Read potentiometer ADC and calculate pendulum angle (theta [rad])
    // // TODO
    // float theta = 0.0f;
    // float result = pot_get_value();
    // float N = POT_FULL_SCALE/240.0;
    // theta = ((result/N)-120.0)*(PI/180.0);

    // // Pop encoder count and update encoder angle (phi [rad])
    // // TODO
    // _phi = _phi + ((float)encoder_pop_count()/2048.0)*(PI*2);

    // // Reference update
    // float ref = reference_update();

    // // Control system update
    // float input[CTRL_N_INPUT];
    // input[0] = ref;                 // Reference cart velocity [m/s]
    // input[1] = _phi;                // Encoder angle measurement [rad]
    // input[2] = theta;               // Pendulum angle measurement [rad]
    // float* U = ctrl_run(input);

    // // Set motor voltage
    // motor_set(U[0]);
    // float Ts = 0.01f;
    // float Kc1 = _Kc1;
    // float Kc2 = _Kc2;
    // float Kc3 = _Kc3;
    // float N = _N;
    // dphi_dem = -Ts*(Kc1*_dtheta + Kc2*(_theta) + Kc3*dphi_dem) + Ts*N*ref;