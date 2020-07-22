#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "motor.h"
#include "motor_cmd.h"

void motor_cmd(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Motor voltage is %.2fV\n", motor_get());
    }
    else if (argc == 2)
    {
        float U = atof(argv[1]);
        motor_set(U);
        // printf("Motor voltage set to %.2fV\n", motor_get());
        printf("%.4f\n",motor_get());
    }
    else
    {
        printf("%s: expecting 0 or 1 arguments, syntax is: %s [<voltage>]\n", argv[0], argv[0]);
    }
}