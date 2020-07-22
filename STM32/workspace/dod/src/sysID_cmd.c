#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sysID_cmd.h"
#include "sysID_task.h"

static SYSID_EXPERIMENT_T _experiment;

void sysID_cmd(int argc, char *argv[])
{
    // Get number of samples
    int n = atoi(argv[2]);
    if (n < 0)
    {
        printf("%s: expecting positive number of samples\n", argv[0]);
        return;
    }

    if (!strcmp(argv[1], "cart"))
    {
        if (argc != 5)
        {
            printf("%s: expecting 4 arguments, syntax is: %s cart <samples> <amplitude> <frequency>\n", argv[0], argv[0]);
            return;
        }
        _experiment = EXP_CART;
        float _amplitude = atof(argv[3]);
        float _frequency = atof(argv[4]);
        sysID_cart_set_param(_amplitude, _frequency);
        printf("Time [sec],Motor Voltage [V],Wheel angle [rad]\n");
        sysID_task_resume(_experiment, n);
    }
    else if (!strcmp(argv[1], "pend"))
    {
        if (argc != 3)
        {
            printf("%s: expecting 2 arguments, syntax is: %s pend <samples>\n", argv[0], argv[0]);
            return;
        }
        _experiment = EXP_PEND;
        printf("Time [sec],Pendulum Angle [rad]\n");
        sysID_task_resume(_experiment, n);
    }
    else if (!strcmp(argv[1], "stop"))
    {
        printf("Experiment stopped\n");
        sysID_task_stop();
    }
    else
    {
        printf("%s: unknown experiment \"%s\", syntax is: %s [cart|pend] <samples> [<amplitude> <frequency>]\n", argv[0], argv[1], argv[0]);
        return;
    }
}