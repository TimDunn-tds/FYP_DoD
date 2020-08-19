#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dc_motor.h"
#include "dc_motor_cmd.h"
#include "dc_motor_task.h"

void dc_motor_cmd(int argc, char *argv[])
{
	if (argc <= 1)
	{
		printf("Motor voltage is %.2fV\n", dc_motor_get());
	}
	else if (argc == 2)
	{
		if (dc_motor_current_task_is_running())
		{
			printf("Current control task is running. Voltage cannot be set\n");
			return;
		}
		else
		{
			float U = atof(argv[1]);
			dc_motor_set(U);
			printf("Motor voltage is set to %.3fV\n", dc_motor_get());
		}
	}
		else if (argc == 3)
	{
		if (dc_motor_current_task_is_running())
		{
			printf("Current control task is running. Voltage cannot be set\n");
			return;
		}
		else
		{
			float U = atof(argv[1]);
			dc_motor_set(U);
			// printf("Motor voltage is set to %.2fV\n", dc_motor_get());
		}
	}
	else
	{
		printf("%s: expecting 0 or 1 arguments, syntax is %s [<voltage>]\n", argv[0], argv[0]);
	}
}
















