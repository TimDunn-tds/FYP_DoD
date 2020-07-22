#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "kalman.h"
#include "kalman_cmd.h"

static float _input[KF_N_INPUT];
static float _meas[3];
static float _output[KF_N_OUTPUT];

void kf_cmd(int argc, char *argv[])
{
    // kf set at %g
    // kf set ar %g
    // kf set gyro %g 
    // kf set u %g
    // kf update meas
    // kf update time
    // kf get mean
    // kf run
    // kf reset

    if (argc == 2)
    {
        if (strcmp(argv[1], "run") == 0)
        {
            kf_run(_input, _output);
            // printf("%g,%g,%g\n", theta[0],theta[1],theta[2]);
        }
        else if (strcmp(argv[1], "reset") == 0)
        {
            kf_init();
        }
    }
    else if (argc == 3)
    {
        if (strcmp(argv[1], "update") == 0)
        {
            if (strcmp(argv[2], "meas") == 0)
            {
                _output[0] = atan2f(_meas[0], _meas[1]);
                _output[1] = _meas[2];
                kf_measurement_update(_input, _output);

            }
            if (strcmp(argv[2], "time") == 0)
            {
                kf_time_update(_input);
            }
        }
        if (strcmp(argv[1], "get") == 0)
        {
            if (strcmp(argv[2], "mean") == 0)
            {
                float* mu = kf_get_mean();
                printf("%g,",mu[0]);
                printf("%g,",mu[1]);
                printf("%g", mu[2]);
                printf("\n");
            }
        }
    }
    else if (argc == 4)
    {
        if (strcmp(argv[1], "set") == 0)
        {
            if (strcmp(argv[2], "at") == 0)
            {
                _meas[0] = atof(argv[3]);
            }
            if (strcmp(argv[2], "ar") == 0)
            {
                _meas[1] = atof(argv[3]);
            }
            if (strcmp(argv[2], "gyro") == 0)
            {
                _meas[2] = atof(argv[3]);
            }
            if (strcmp(argv[2], "u") == 0)
            {
                // _input[0] = atof(argv[3]);
            }
        }
        else
        {
            printf("%s: invalid argument \"%s\"\n", argv[0], argv[1]);
        }
    }
    else
    {
        printf("%s: expecting 1, 2 or 3 arguments\n", argv[0]);
    }
}