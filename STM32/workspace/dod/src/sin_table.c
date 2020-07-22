#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "sin_table.h"
#define PI 3.141592653589793

static uint32_t _n_samples;
static float _time, _amplitude, _frequency;
static osTimerId_t timer_id = NULL;
osStatus_t status;
uint32_t arg_in;



void sin_table_cmd(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("%s: expecting 3 arguments, syntax is: %s <samples> <amplitude> <frequency>\n", argv[0], argv[0]);
        return;
    }

    _n_samples = atoi(argv[1]);
    _amplitude = atof(argv[2]);
    _frequency = atof(argv[3]);
    _time      = 0.0f;

    printf("Time [sec],Value\n");

    // DONE: On first call, create the timer
    // Hint: statically initialise the timerid to NULL.
    //       If it's NULL, call osTimerCreate.???
    if (timer_id == NULL)
    {
        timer_id = osTimerNew(sin_table_task, osTimerPeriodic, &arg_in, NULL);
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

void sin_table_task(void *arg)
{
    UNUSED(arg);
    // DONE: calculate and print one row of data
    // Hint: Use sinf, not sin
    printf("%f,%f\n", _time, (_amplitude*sinf(2*PI*_frequency*_time)));

    _time += 0.01f;

    if (--_n_samples == 0)
    {
        // DONE: stop the timer
        osTimerStop(timer_id);
    }
}