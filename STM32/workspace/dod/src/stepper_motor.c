#include "stm32f4xx_hal.h"
#include "stepper_motor.h"
#include "math.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "cmsis_os2.h"

#define PI 3.141592653589793
#define SPR 800.0f
#define PRSC_LOW 999
#define PRSC_HIGH 9999

static int _stepper_status = 0;

static float rev_per_sec, steps_per_sec, rad_per_sec;
static uint32_t steps_remaining;
uint32_t arg_in;

// Pulse is the pulse width.
// Period is the frequency.
// Prescaler is???
// Fidelity loss occurs above 4pi rad/sec
// minimum velocity availiable is 0.012 rad/sec

TIM_HandleTypeDef _htim3;
TIM_HandleTypeDef _htim2;

static uint8_t _is_init = 0;


void stepper_motor_init(void)
{	

	stepper_motor_gpio_init_2();
	// test();


}


void stepper_motor_gpio_init_2(void)
{

	// Enable GPIO Clocks
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();


	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OC_InitTypeDef sConfigPWM;

	// Init Stepper driver 1 direction pin - PA11
	GPIO_InitStructure.Pin 			= GPIO_PIN_11;
	GPIO_InitStructure.Mode 		= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull 		= GPIO_PULLUP;
	GPIO_InitStructure.Speed 		= GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);


	// Init stepper driver 2 direction pin - PB7
	GPIO_InitStructure.Pin 			= GPIO_PIN_7;
	GPIO_InitStructure.Mode 		= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull 		= GPIO_PULLUP;
	GPIO_InitStructure.Speed 		= GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);


	// Init stepper driver 1 step pin with pwm timer - PC8
	GPIO_InitStructure.Pin  		= GPIO_PIN_8;
	GPIO_InitStructure.Mode 		= GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull 		= GPIO_PULLUP;
	GPIO_InitStructure.Speed 		= GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Alternate 	= GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Init the timer itself
	_htim3.Instance = TIM3;
    _htim3.Init.Prescaler = PRSC_LOW;
    _htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    _htim3.Init.Period = 499;
    _htim3.Init.ClockDivision = 0;


    sConfigPWM.OCMode = TIM_OCMODE_PWM2;
    sConfigPWM.Pulse = 1;
    sConfigPWM.OCPolarity = TIM_OCPOLARITY_LOW;
    sConfigPWM.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_Init(&_htim3);
	HAL_TIM_PWM_ConfigChannel(&_htim3, &sConfigPWM, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&_htim3, TIM_CHANNEL_3);


	// Init stepper driver 2 step pin with pwm timer - PA15
	GPIO_InitStructure.Pin  		= GPIO_PIN_15;
	GPIO_InitStructure.Mode 		= GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull 		= GPIO_PULLUP;
	GPIO_InitStructure.Speed 		= GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Alternate 	= GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);

	// Init the timer itself
	_htim2.Instance = TIM2;
    _htim2.Init.Prescaler = PRSC_LOW;
    _htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    _htim2.Init.Period = 499;
    _htim2.Init.ClockDivision = 0;

    // memset(&sConfigPWM,0,sizeof(sConfigPWM));

    sConfigPWM.OCMode = TIM_OCMODE_PWM2;
    sConfigPWM.Pulse = 1;
    sConfigPWM.OCPolarity = TIM_OCPOLARITY_LOW;
    sConfigPWM.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_Init(&_htim2);
	HAL_TIM_PWM_ConfigChannel(&_htim2, &sConfigPWM, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&_htim2, TIM_CHANNEL_1);


	/* The microstepping pins are:
	MS1 <---> PA5
	MS2 <---> PA6
	MS3 <---> PA7 */
	/* Init microstepping pins */
	GPIO_InitStructure.Pin 		= GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStructure.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull 	= GPIO_PULLDOWN;
	GPIO_InitStructure.Speed 	= GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);	

	/* Set pins for FULL? STEP */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);

	/* Set both motors going forward for the time being */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

}



void stepper_motor_step(uint16_t freq, uint16_t prsc)
{
	// TIM_HandleTypeDef _htim2;
	// TIM_HandleTypeDef _htim3;
	HAL_TIM_PWM_Stop(&_htim2, TIM_CHANNEL_1);
	__HAL_TIM_SET_COUNTER(&_htim2,0);
	__HAL_TIM_SET_AUTORELOAD(&_htim2,freq);
	__HAL_TIM_SET_PRESCALER(&_htim2, prsc);
	HAL_TIM_PWM_Start(&_htim2, TIM_CHANNEL_1);


	HAL_TIM_PWM_Stop(&_htim3, TIM_CHANNEL_3);
	__HAL_TIM_SET_COUNTER(&_htim3,0);
	__HAL_TIM_SET_AUTORELOAD(&_htim3,freq);
	__HAL_TIM_SET_PRESCALER(&_htim3, prsc);
	HAL_TIM_PWM_Start(&_htim3, TIM_CHANNEL_3);

	// float disp = __HAL_TIM_GET_AUTORELOAD(&_htim2);
	// printf("%f\n",disp);
}


void stepper_set_dir(int argc, char*argv[])
{
	int dir1 = atoi(argv[1]);
	int dir2 = atoi(argv[2]);
	// dir pins are pa11 for sm1 and pb7 for sm2
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, dir1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, dir2);
	printf("Setting directions to %i and %i\n", dir1, dir2);


}

void stepper_set_speed(int argc, char*argv[])
{
	if (argc != 2)
	{
		printf("%s: expecting 1 argument, syntax is %s <rad/s>\n", argv[0], argv[0]);
		return;
	}

	rad_per_sec = atof(argv[1]);
	rev_per_sec = rad_per_sec/(2.0f*PI);
	printf("rad per sec: %f\n", rad_per_sec);
	printf("rev per sec: %f\n", rev_per_sec);
	steps_per_sec = rev_per_sec*SPR;
	float timer_freq = (rev_per_sec*SPR);
	// timer_freq = round(timer_freq);

	printf("Now attemping to spin at %f rev/s\n", rev_per_sec);
	printf("Timer Freq is %f\n",timer_freq);


	float period = (100000000/(1000*timer_freq)) - 1;
	period = round(period);

	// uint16_t period = (100000000/(1000*timer_freq)) - 1;
	printf("Period is: %f\n",period);
	stepper_motor_step(period, 999);

}

void stepper_turn_off(int argc, char*argv[])
{	
	UNUSED(argv);
	UNUSED(argc);
	HAL_TIM_PWM_Stop(&_htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&_htim3, TIM_CHANNEL_3);
	printf("Stopping stepper pwm signal\n");
	_stepper_status = 0;
}

void stepper_turn_on(int argc, char*argv[])
{
	UNUSED(argv);
	UNUSED(argc);
	HAL_TIM_PWM_Start(&_htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&_htim3, TIM_CHANNEL_3);
	printf("Starting stepper pwm signal\n");
	_stepper_status = 1;

}

float get_steps_per_rev(void)
{
	return SPR;
}


void stepper_ctrl_speed(float dphi)
{
	// if (dphi>4*PI)
	// {
	// 	// HAL_TIM_PWM_Stop(&_htim2, TIM_CHANNEL_1);
	// 	// HAL_TIM_PWM_Stop(&_htim3, TIM_CHANNEL_3);
	// 	// printf("Stopping stepper pwm signal\n");

	// 	// return 0.0f;
		// dphi = 8.0f*PI;
	// }
	if (abs(dphi)>30.0f)
	{
		stepper_turn_off(0,NULL);
		return;
	}

	else if(!_stepper_status)
	{
		stepper_turn_on(0,NULL);
	}

	else if (dphi>0.0f)
	{
		stepper_forward();
		// printf("forward\n");
	}
	else if (dphi<0.0f)
	{
		stepper_backward();
		dphi = -dphi;
		// printf("backward\n");
	}

	if (dphi<0.012f)
	{
		rad_per_sec = dphi;
		rev_per_sec = rad_per_sec/(2.0f*PI);
		// printf("rad per sec: %f\n", rad_per_sec);
		// printf("rev per sec: %f\n", rev_per_sec);
		steps_per_sec = rev_per_sec*SPR;
		float timer_freq = (rev_per_sec*SPR);
		// timer_freq = round(timer_freq);

		// printf("Now attemping to spin at %f rev/s\n", rev_per_sec);
		// printf("Timer Freq is %f\n",timer_freq);


		float period = (100000000.0f/((PRSC_HIGH+1.0f)*timer_freq)) - 1.0f;
		period = round(period);

		// uint16_t period = (100000000/(1000*timer_freq)) - 1;
		// printf("Period is: %f\n",period);
		stepper_motor_step(period,PRSC_HIGH);
		return;
	}

	else 
	{
		rad_per_sec = dphi;
		rev_per_sec = rad_per_sec/(2.0f*PI);
		// printf("rad per sec: %f\n", rad_per_sec);
		// printf("rev per sec: %f\n", rev_per_sec);
		steps_per_sec = rev_per_sec*SPR;
		float timer_freq = (rev_per_sec*SPR);
		// timer_freq = round(timer_freq);
	
		// printf("Now attemping to spin at %f rev/s\n", rev_per_sec);
		// printf("Timer Freq is %f\n",timer_freq);
	
	
		float period = (100000000.0f/((PRSC_LOW+1.0f)*timer_freq)) - 1.0f;
		period = round(period);
	
		// uint16_t period = (100000000/(1000*timer_freq)) - 1;
		// printf("Period is: %f\n",period);
		stepper_motor_step(period,PRSC_LOW);
		return;
	}

}

void stepper_backward(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
}

void stepper_forward(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
}

float stepper_ctralloc(float dphi)
{
	if (dphi>8.0f*PI)
	{
		dphi = 8.0f*PI;
	}

	return dphi;
}