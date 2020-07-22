#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "led_rgb.h"

TIM_HandleTypeDef _htim3;

static uint8_t _is_init = 0;

void led_rgb_init(void)
{
    if (!_is_init)
    {
        /* Init PC6(TIM3_CH1), PC8(TIM3_CH3), PC9(TIM3_CH4) as GPIO outputs for `Hello World' */
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_TIM3_CLK_ENABLE();

        /* Configure PC6, PC8 & PC9 in AF pushpull mode with TIM3 alternate function*/
        GPIO_InitTypeDef  GPIO_InitStructure;
        GPIO_InitStructure.Pin = GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_9;
        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructure.Pull = GPIO_PULLUP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStructure.Alternate = GPIO_AF2_TIM3;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

        // Configure Timer3 to generate three PWM outputs on PC6(TIM3_CH1),PC8(TIM3_CH3),PC9(TIM3_CH4).
        _htim3.Instance = TIM3;
        _htim3.Init.Prescaler = 1;
        _htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
        _htim3.Init.Period = 65535;
        _htim3.Init.ClockDivision = 0;

        TIM_OC_InitTypeDef sConfigPWM;
        sConfigPWM.OCMode = TIM_OCMODE_PWM2;
        sConfigPWM.Pulse = 0;
        sConfigPWM.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigPWM.OCFastMode = TIM_OCFAST_DISABLE;

        HAL_TIM_PWM_Init(&_htim3);
        HAL_TIM_PWM_ConfigChannel(&_htim3, &sConfigPWM, TIM_CHANNEL_1);
        HAL_TIM_PWM_ConfigChannel(&_htim3, &sConfigPWM, TIM_CHANNEL_3);
        HAL_TIM_PWM_ConfigChannel(&_htim3, &sConfigPWM, TIM_CHANNEL_4);

        HAL_TIM_PWM_Start(&_htim3, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&_htim3, TIM_CHANNEL_3);
        HAL_TIM_PWM_Start(&_htim3, TIM_CHANNEL_4); 

        _is_init = 1;
    }

    led_rgb_set_red(0);
    led_rgb_set_green(0);
    led_rgb_set_blue(0);
}

void led_rgb_deinit(void)
{
    _is_init = 0;
}

uint16_t led_rgb_get_red(void) 
{
    return __HAL_TIM_GET_COMPARE(&_htim3, TIM_CHANNEL_1);
}
uint16_t led_rgb_get_green(void)
{
    return __HAL_TIM_GET_COMPARE(&_htim3, TIM_CHANNEL_3);
}
uint16_t led_rgb_get_blue(void)
{
    return __HAL_TIM_GET_COMPARE(&_htim3, TIM_CHANNEL_4);
}
void led_rgb_set_red(uint16_t R)
{
    __HAL_TIM_SET_COMPARE(&_htim3, TIM_CHANNEL_1, R);
}
void led_rgb_set_green(uint16_t G)
{
    __HAL_TIM_SET_COMPARE(&_htim3, TIM_CHANNEL_3, G);
}
void led_rgb_set_blue(uint16_t B)
{
    __HAL_TIM_SET_COMPARE(&_htim3, TIM_CHANNEL_4, B);
}