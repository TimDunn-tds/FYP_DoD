#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "dc_motor.h"
#include "adc_cmd.h"


void adc_cmd(int argc, char *argv[])
{
	if (argc <= 1)
	{
        printf("ADC value is %" PRIu16 "\n", dc_adc_get_value());
	}
	else
	{
		printf("%s: expecting 0 arguments.\n", argv[0]);
	}
}

