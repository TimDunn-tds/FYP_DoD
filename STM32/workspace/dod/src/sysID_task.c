#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "math.h"
#include "button.h"
#include "led_rgb.h"
#include "light.h"
#include "potentiometer.h"
#include "encoder.h"
#include "dimmer_task.h"
#include "motor.h"
#include "reference.h"
#include "sysID_task.h"
#define PI 3.141592653589793

static osTimerId_t _sysID_task_TimerID;

static void _sysID_task_update(void *argument);

static uint8_t _is_running = 0;
static uint8_t _is_init = 0;

static SYSID_EXPERIMENT_T _experiment;
static uint32_t _n_samples;
static float _time = 0.0f;

// Cart variables
static float _amplitude;
static float _frequency;
static float _phi = 0.0f;
// Pend variables
static float _theta;

void sysID_task_init(void)
{
    if (!_is_init)
    {
        // Initialise GPIO PA12 for sync pin : Active Low
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitTypeDef  GPIO_InitStructure;
        GPIO_InitStructure.Pin      = GPIO_PIN_12;
        GPIO_InitStructure.Mode     = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Pull     = GPIO_NOPULL;
        GPIO_InitStructure.Speed    = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);

        if (_sysID_task_TimerID == NULL)
        {
            _sysID_task_TimerID = osTimerNew(_sysID_task_update, osTimerPeriodic, (void *)0, NULL);
        }

        _is_init = 1;
    }
}

void sysID_task_resume(SYSID_EXPERIMENT_T experiment, uint32_t n_samples)
{
    if (!osTimerIsRunning(_sysID_task_TimerID))
    {
        // Check if dimmer is running and stop it
        if (dimmer_task_is_running())
        {
            dimmer_task_stop();
            printf("Dimmer has stopped\n");
        }

        if(experiment == EXP_CART)
        {
            encoder_init();
            motor_init();
            // Reset encoder
            encoder_set_count(0);
            // Clear phi 'integrator'
            _phi = 0.0f;
        }
        else if(experiment == EXP_PEND)
        {
            pot_init();
            _theta = 0.0f;
        }
        else
        {
            // Shouldn't get here
            printf("Invalid experiment\n");
            return;
        }

        _experiment = experiment;
        _n_samples = n_samples;
        _time = 0.0f;

        // Clear PA12 to sync dSPACE simulation
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);

        osTimerStart(_sysID_task_TimerID, 10);
        _is_running = 1;
    }
}

void sysID_task_stop(void)
{
    if (_is_running)
    {
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);

        // Set PA12 to stop dSPACE simulation
        osTimerStop(_sysID_task_TimerID);
        _is_running = 0;
    }
}

uint8_t sysID_task_is_running(void)
{
    return _is_running;
}

void sysID_task_deinit(void)
{
    _is_init = 0;
    _is_running = 0;
}

void _sysID_task_update(void *arg)
{
    UNUSED(arg);
    if(_experiment == EXP_CART)
    {
        // Set motor voltage for given amplitude and frequency sin wave and current time
        float U = 0.0f; // DONE: Implement this signal. Hint: See sin table implementation from Lab 4
        U = _amplitude*sinf(2*PI*_frequency*_time);
        motor_set(U);
        // Read encoder and update phi
        // DONE: Set/update _phi
        _phi = _phi + ((float)encoder_pop_count()/2048.0)*(PI*2);

        printf("%.2f,%.2f,%.2f\n", _time, U, _phi);
        // TODO: Set/update _phi
        printf("%.2f,%g,%g\n", _time, U, _phi);
    }
    else if(_experiment == EXP_PEND)
    {
        // Read potentiometer to calculate pend. angle
        // DONE: Set/update _theta
    	float result = pot_get_value();
    	float N = POT_FULL_SCALE/240.0;
    	_theta = ((result/N)-120.0)*(PI/180.0);
        printf("%.2f,%g\n", _time, _theta);
    }
    else
    {
        // Shouldn't get here
        printf("Invalid experiment\n");
        sysID_task_stop();
        return;
    }

    _time += 0.01f;

    if (--_n_samples == 0)
    {
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
        sysID_task_stop();
    }
}

void sysID_cart_set_param(float amplitude, float frequency)
{
    _amplitude = amplitude;
    _frequency = frequency;
}