#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dc_motor.h"
#include "cc_cmd.h"
#include "dc_motor_task.h"

void cc_cmd(int argc, char *argv[])
{
	if (argc <= 1)
	{
		printf("Motor current is %.4fA\n", dc_motor_get_current());
	}
	else if (argc == 2)
	{
		if (dc_motor_current_task_is_running())
		{
			float desired_current = atof(argv[1]);
			dc_motor_current_task_stop();
			dc_motor_set_desired_current(desired_current);
			dc_motor_current_task_resume();
			// printf("Motor current is set to %.3fA\n", desired_current);
			printf("Motor current is set to %.3fA\n", dc_motor_get_desired_current());
		}
		else
		{
			printf("Current control task is not running\n");
		}
	}
	else if (argc == 3)
	{
		if (dc_motor_current_task_is_running())
		{
			float desired_current = atof(argv[1]);
			dc_motor_current_task_stop();
			dc_motor_set_desired_current(desired_current);
			dc_motor_current_task_resume();
			// printf("Motor voltage is set to %.2fV\n", dc_motor_get());
		}
		else
		{
			printf("Current control task is not running\n");
		}

	}
	else
	{
		printf("%s: expecting 0 or 1 arguments, syntax is %s [<voltage>]\n", argv[0], argv[0]);
	}
}

