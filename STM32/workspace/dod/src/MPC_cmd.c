#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MPC_cmd.h"
#include "MPC_task.h"

void MPC_cmd(int argc, char *argv[])
{
    if (argc <= 1)
    {
        if (MPC_task_is_running())
            printf("MPC controller is currently running. Better catch it.\n");
        else
            printf("MPC controller is not currently running.\n");
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "start") == 0)
        {
            MPC_task_resume();
            printf("MPC controller has started\n");
        }
        else if (strcmp(argv[1], "stop") == 0)
        {
            MPC_task_stop();
            printf("MPC controller has stopped\n");
        }
        else
        {
            printf("%s: invalid argument \"%s\", syntax is: %s [start|stop]\n", argv[0], argv[1], argv[0]);
        }
    }
    else
    {
        printf("%s: expecting 0 or 1 arguments, syntax is: %s [start|stop]\n", argv[0], argv[0]);
    }
}