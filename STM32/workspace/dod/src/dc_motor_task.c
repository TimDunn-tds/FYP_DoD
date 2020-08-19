#include <stdint.h>
#include <math.h>
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "dc_motor.h"
#include "dc_motor_task.h"


#define K   2.5f
#define K2  1.0f

static osThreadId_t _dc_motor_current_task_thread_id;
static osThreadAttr_t dc_motor_current_task_attr = 
{
    .name = "current_task",
    .priority = osPriorityNormal,
};

static void _dc_motor_current_task_update(void *argument);
static float _sign(float x);

static uint8_t _is_running = 0;
static uint8_t _is_init = 0;
static float _desired_current = 0.0f;
static float _sum = 0.0f;
static int _count = 1;
static float _error = 0.0f;
static int _motor_started = 0;

void dc_motor_current_task_init(void)
{
    if (!_is_init)
    {
        _dc_motor_current_task_thread_id = osThreadNew(_dc_motor_current_task_update, NULL, &dc_motor_current_task_attr);
        _is_running = 1;
        _is_init = 1;
    }
    dc_motor_current_task_stop();
}

void dc_motor_current_task_resume(void)
{
    if (!_is_running)
    {
        osThreadResume(_dc_motor_current_task_thread_id);
        _is_running = 1;
    }
}

void dc_motor_current_task_stop(void)
{
    if (_is_running)
    {
        osThreadSuspend(_dc_motor_current_task_thread_id);
        _is_running = 0;
    }
}

uint8_t dc_motor_current_task_is_running(void)
{
    return _is_running;
}

void _dc_motor_current_task_update(void *argument)
{
    UNUSED(argument);
    float voltage = 0;
    float current = 0;
    while(1)
    {
        if (dc_motor_get_count == 0)
        {
            motor_set(2);
            continue;
        }
        if (_count<10)
        {
            _sum = _sum + dc_motor_get_current();
            _count ++;
        }
        else if (_count == 10)
        {
            // get desired current
            _desired_current = dc_motor_get_desired_current();
            // current = dc_motor_get_current();
            current = _sum/(float)_count;
            _error = _error + (current - _desired_current);

            // voltage = -(K*(current - _desired_current) + K2*_error);
            voltage = -(K2*_error);

            // Set motor voltage
            if (fabsf(voltage) > 6.0f)
            {
                voltage = 6.0f * _sign(voltage);
            }

            printf("IaD = %1.2f, Ia = %1.4f, Vd = %2.4f, errSum = %4.4f\n", _desired_current, current, dc_motor_get(), _error);

            if (_desired_current == 0)
            {
                dc_motor_set(0);
            }
            else
            {
                    dc_motor_set(voltage);
            }

            _count = 0;
            _sum = 0.0f;
        }
        // step 5: osdelay
        osDelay(50);

    }
}

void dc_motor_current_task_deinit(void)
{
    _is_init = 0;
    _is_running = 0;
}

float _sign(float x)
{
    return (x > 0.0f) - (x < 0.0f);
}