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
// #include "mpu6050.h"
#include "log_data.h"
#include "kalman.h"



static float _input[KF_N_INPUT];
static float _meas[3];
static float _output[KF_N_OUTPUT];


typedef enum {
    LOG_POT,
    LOG_ENC,
    LOG_IMU_RAW
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
    else
    {
        printf("%s: unknown device \"%s\", syntax is: %s [pot|enc] <samples>\n", argv[0], argv[1], argv[0]);
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
        status = osTimerStart(timer_id,10);
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



        // MPU6050_read();
        // printf("%.2f, ", _time);
        // printf("%6f, ", MPU6050_get_ax());
        // printf("%6f, ", MPU6050_get_ay());
        // printf("%6f, ", MPU6050_get_az());
        // printf("%6f, ", MPU6050_get_gx());
        // printf("%6f, ", MPU6050_get_gy());
        // printf("%6f, ", MPU6050_get_gz());
        // printf("%" PRId32 "\n", encoder_get_count());   


        break;
    default:
        printf("Invalid device\n");
        // DONE: stop the timer
        osTimerStop(timer_id);

        return;
    }

    _time += 0.01f;

    if (--_n_samples == 0)
    {
        // DONE: stop the timer
        osTimerStop(timer_id);

    }
}