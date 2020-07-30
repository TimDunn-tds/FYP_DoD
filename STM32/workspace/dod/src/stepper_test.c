#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "stepper_test.h"
#include "stepper_motor.h"
#define PI 3.141592653589793

static float rev_per_sec, steps_per_sec;
static uint32_t steps_remaining;
static osTimerId_t timer_id = NULL;
osStatus_t status;
uint32_t arg_in;

void stepper_test_cmd(int argc, char*argv[])
{
	if (argc != 2)
	{
		printf("%s: expecting 1 argument, syntax is %s <rev/s>\n", argv[0], argv[0]);
		return;
	}

	rev_per_sec = atof(argv[1]);
	steps_per_sec = rev_per_sec*200.0f;
	float timer_freq = (rev_per_sec*200.0f);
	timer_freq = round(timer_freq);

	printf("Now attemping to spin at %f rev/s\n", rev_per_sec);
	printf("Timer Freq is %f\n",timer_freq);

	uint16_t period = (100000000/(1000*timer_freq)) - 1;
	printf("Period is: %i\n",period);
	stepper_motor_step(period, 999);



}







