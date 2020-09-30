#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "potentiometer.h"
#include "encoder.h"
#include "dimmer_task.h"
#include "mpu6050.h"
#include "log_data.h"
#include "kalman.h"
#include "dc_motor.h"
#include "control_allocation.h"

#define COUNT_PER_REV 4741.44f
#define PI 3.141592653589793f
// #define TIMER_TICKS 25
#define TIMER_TICKS 10

#define MVPA 0.14f



static float _input[KF_N_INPUT];
static float _meas[3];
static float _output[KF_N_OUTPUT];
static int32_t _count = 0;
static float _voltage = 0.0f;
static float _position = 0.0f;
static float _last_position = 0.0f;
static float _speed = 0.0f;
static float _current = 0.0f;


typedef enum {
    LOG_POT,
    LOG_ENC,
    LOG_IMU_RAW,
    LOG_DC
} LOG_DEVICE_T;

static uint32_t _n_samples;
static LOG_DEVICE_T _device;
static float _time;
static osTimerId_t timer_id = NULL;
osStatus_t status;
uint32_t arg_in;

void log_data_cmd(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("%s: expecting 2 arguments, syntax is: %s [pot|enc] <samples>\n", argv[0], argv[0]);
        return;
    }

    int n = atoi(argv[2]);
    if (n < 0)
    {
        printf("%s: expecting positive number of samples\n", argv[0]);
        return;
    }
    _n_samples = n;
    _time = 0.0f;

    if (!strcmp(argv[1], "pot"))
    {
        _device = LOG_POT;
        pot_init();
        printf("Time [sec],Potentiometer [V]\n");

    }
    else if (!strcmp(argv[1], "enc"))
    {
        _device = LOG_ENC;

        if (dimmer_task_is_running())
        {
            dimmer_task_stop();
            printf("Dimmer has stopped\n");
        }

        encoder_init();
        printf("Time [sec],Encoder [-]\n");
    }
    else if (!strcmp(argv[1], "IMU"))
    {
        _device = LOG_IMU_RAW;

        if (dimmer_task_is_running())
        {
            dimmer_task_stop();
            // printf("Dimmer has stopped\n");
        }
        MPU6050_init();
        // encoder_init();
        // printf("Time [sec],ax [-],ay [-],az [-],gx [-],gy [-],gz [-]\n");
        kf_init();
    }
    else if (!strcmp(argv[1],"DC")||!strcmp(argv[1],"dc"))
    {
        _device = LOG_DC;
        if (dimmer_task_is_running())
        {
            dimmer_task_stop();
            printf("Dimmer has stopped\n");
        }
        encoder_init();
        _last_position = 0.0f;
        printf("Time [sec], Encoder [-], Position [rad], Speed [rad/s], Voltage [V], Current [A]\n");
    }
    else if (!strcmp(argv[1],"ctrl"))
    {
        // Get encoder count
        _count = dc_motor_get_count();

        // position = (count/steps_per_rev) * 2pi
        _position = dc_motor_get_position();

        // remember to conver timer ticks to seconds
        // _speed = (_position-_last_position)/((float)TIMER_TICKS/1000.0f);
        _speed = ctrl_get_speed();

        _voltage = dc_motor_get_voltage();

        _current = dc_motor_get_current();

        // printf("%12i,%15.4f,%14.6f,%12.4f,%12.2f\n", _count, _position, _speed, _voltage, _current);
        printf("%15.4f,%14.6f\n", _position, _speed);

        _last_position = _position;
        return;
    }
    else if (!strcmp(argv[1],"reset"))
    {
        encoder_set_count(0);
        _last_position = 0.0f;
        _position = 0.0f;
        _speed = 0.0f;
        _voltage = 0.0f;
        _current = 0.0f;
        return;
    }
    else
    {
        printf("%s: unknown device \"%s\", syntax is: %s [pot|enc|IMU|dc|ctrl] <samples>\n", argv[0], argv[1], argv[0]);
        return;
    }

    // DONE: On first call, create the timer
    // Hint: statically initialise the timerid to NULL.
    //       If it's NULL, call osTimerCreate.???
    if (timer_id == NULL)
    {
        timer_id = osTimerNew(log_data_task, osTimerPeriodic, &arg_in, NULL);
    }

    // DONE: If the timer isn't running, start the timer.
    if (osTimerIsRunning(timer_id) == 0)
    {
        status = osTimerStart(timer_id,TIMER_TICKS);
        if (status != osOK)
        {
            printf("Timer could not be started.\n");
        }
    }

 
}

void log_data_task(void *arg)
{
    UNUSED(arg);

    switch (_device)
    {
    case LOG_POT:
        printf("%.2f,%g\n", _time, (float)pot_get_value()*3.3f/POT_FULL_SCALE);
        break;
    case LOG_ENC:
        printf("%.2f,%" PRId32 "\n", _time, encoder_get_count());
        break;
    case LOG_IMU_RAW:
        // printf("dtheta\t\t, theta\t\t, bias\n");

        MPU6050_read();
        _meas[0] = MPU6050_get_az();
        _meas[1] = MPU6050_get_ay();
        _meas[2] = MPU6050_get_gx();

        _output[0] = atan2f(-_meas[0], _meas[1]);
        _output[1] = _meas[2];
        kf_measurement_update(_input,_output);
        kf_time_update(_input);
        float* out = kf_get_mean();
        printf("%.5g, %.5g, %.5g\n", out[0],out[1],out[2]);
        break;
    case LOG_DC:

    // Get encoder count
    _count = dc_motor_get_count();

    // position = (count/steps_per_rev) * 2pi
    _position = dc_motor_get_position();

    // remember to conver timer ticks to seconds
    _speed = (_position-_last_position)/((float)TIMER_TICKS/1000.0f);

    _voltage = dc_motor_get_voltage();

    _current = dc_motor_get_current();

    printf("%10.2f,%12i,%15.4f,%14.6f,%12.4f,%12.2f\n", _time, _count, _position, _speed, _voltage, _current);
    _last_position = _position;

        break;



    default:
        printf("Invalid device\n");
        // DONE: stop the timer
        osTimerStop(timer_id);

        return;
    }

    _time += TIMER_TICKS/1000.0f;

    if (--_n_samples == 0)
    {
        // DONE: stop the timer
        osTimerStop(timer_id);

    }
}