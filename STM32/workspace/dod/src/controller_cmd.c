#include <stdio.h>
#include <string.h>
#include "controller_task.h"
#include "controller_cmd.h"

void controller_cmd(int argc, char *argv[])
{
    if (argc <= 1)
    {
        if (controller_task_is_running())
            printf("Controller is currently running. Better catch it.\n");
        else
            printf("Controller is not currently running.\n");
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "start") == 0)
        {
            controller_task_resume();
            printf("Controller has started\n");
        }
        else if (strcmp(argv[1], "stop") == 0)
        {
            controller_task_stop();
            printf("Controller has stopped\n");
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