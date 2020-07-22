#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "reference.h"
#include "reference_cmd.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>


void manual_reference_cmd(int argc, char *argv[])
{
	if (argc == 2)
	{	
		float ref = atof(argv[1]);
		printf("%f\n", ref);
		manual_reference(ref);
	}
}