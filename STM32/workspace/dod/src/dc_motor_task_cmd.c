#include <stdio.h>
#include <string.h>
#include "dc_motor_task.h"
#include "dc_motor_task_cmd.h"
int _is_running;
// CLB_CREATE_STATIC(clb, 80);

void dc_motor_task_cmd(int argc, char *argv[])
{
    if (argc <= 1)
    {
        _is_running=dc_motor_current_task_is_running();
        if (_is_running==0)
        {
			printf("Current control task is not currently running.\n");

        }
        else 
        {
			printf("Current control task is currently running. Better catch it.\n");

        }

    }
    else
    {
        if (strcmp(argv[1], "start") == 0)
        {
        	dc_motor_current_task_resume();
        	printf("Current control task has started\n");
        }
        else if (strcmp(argv[1], "stop") == 0)
        {
            dc_motor_current_task_stop();
            printf("Current control task has stopped\n");
        }
        else
        {
            printf("%s: invalid argument \"%s\", syntax is: %s [start|stop]\n", argv[0], argv[1], argv[0]);
        }
    }
}