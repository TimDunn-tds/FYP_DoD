#include <stdio.h>
#include <string.h>
#include "dimmer_task.h"
#include "dimmer_cmd.h"
int _is_running;
// CLB_CREATE_STATIC(clb, 80);

void dimmer_cmd(int argc, char *argv[])
{
    if (argc <= 1)
    {
        // DONE: print message according to the return value of dimmer_task_is_running()
        _is_running=dimmer_task_is_running();
        if (_is_running==0)
        {
			printf("Dimmer is not currently running.\n");

        }
        else 
        {
			printf("Dimmer is currently running. Better catch it.\n");

        }

    }
    else
    {
        if (strcmp(argv[1], "start") == 0)
        {
            // TODO: start/resume dimmer task and print appropriate message
        	dimmer_task_resume();
        	printf("Dimmer has started\n");

        	// clb_process(&test);
        }
        else if (strcmp(argv[1], "stop") == 0)
        {
            // TODO: stop dimmer task and print appropriate message
            dimmer_task_stop();
            printf("Dimmer has stopped\n");
        }
        else
        {
            printf("%s: invalid argument \"%s\", syntax is: %s [start|stop]\n", argv[0], argv[1], argv[0]);
        }
    }
}