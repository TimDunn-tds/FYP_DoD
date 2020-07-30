#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "dc_motor.h"
#include "math.h"


#define     MOTOR_DIR_FWD   1
#define     MOTOR_DIR_BCK   2
#define     MOTOR_DIR_OFF   3

ADC_HandleTypeDef _hadc1;

static TIM_HandleTypeDef   _htim4;

float UMAX = 12.0f;
int PERIOD = 65535;
float _voltage = 0.0f;
int pulseWidth = 0;
float ADC_MAX = 0xFFF0;

void dc_motor_init(void)
{
	// Enable Clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_TIM4_CLK_ENABLE();

    GPIO_InitTypeDef  GPIO_InitStructure;


    // Configure PC10 as INA (output)
    GPIO_InitStructure.Pin = GPIO_PIN_10;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    // Configure PC12 as INB (output NOPULL)
    GPIO_InitStructure.Pin = GPIO_PIN_12;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    // Configure PB7 TIM 4/2  as PWM (output pushpull)
	GPIO_InitStructure.Pin = GPIO_PIN_7;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Configure the associated timer
    _htim4.Instance = TIM4;
    _htim4.Init.Prescaler = 0;
    _htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    _htim4.Init.Period = PERIOD;
    _htim4.Init.ClockDivision = 1;

    TIM_OC_InitTypeDef  _sConfigPWM;
    _sConfigPWM.OCMode = TIM_OCMODE_PWM1;
    _sConfigPWM.Pulse = 1;
    _sConfigPWM.OCPolarity = TIM_OCPOLARITY_HIGH;
    _sConfigPWM.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_Init(&_htim4);
    HAL_TIM_PWM_ConfigChannel(&_htim4, &_sConfigPWM, TIM_CHANNEL_2);

    /* Start Timer4 */
    HAL_TIM_PWM_Start(&_htim4, TIM_CHANNEL_2);

}

void dc_adc_init(void)
{
    // Initialise DC motor voltage in pin
    // PC5 ADC IN 1
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_ADC1_CLK_ENABLE();

    /* Configure ADC1 instance to be initialised with:
            div2 prescaler,
            12-bit resolution, 
            right data aligned, 
            continuous conversion mode,
            one conversion. */
    _hadc1.Instance = ADC1;
    _hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    _hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    _hadc1.Init.DataAlign = ADC_DATAALIGN_LEFT;
    _hadc1.Init.ContinuousConvMode = ENABLE;
    _hadc1.Init.NbrOfConversion = 1;

    /* Configure the ADC to:
            use channel 15 because thats what PC5 is on,
            sequence rank of 1,
            480 cycle sample time,
            offset of 0.                            */
    ADC_ChannelConfTypeDef sConfigADC;
    sConfigADC.Channel = ADC_CHANNEL_15;
    sConfigADC.Rank = 1;
    sConfigADC.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    sConfigADC.Offset = 0;

    /* Configure PC5 in analog mode, no pullup */
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_5;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

        /* Initialise the ADC */ 
    if(HAL_ADC_Init(&_hadc1) != HAL_OK)
    {
        printf("Error initialising ADC! \n");
        return;
    }
    /* Initialise the ADC channel config */
    if(HAL_ADC_ConfigChannel(&_hadc1, &sConfigADC) != HAL_OK)
    {
        printf("Error configuring ADC channel!\n");
        return;
    }
    /* Start the ADC */
    if(HAL_ADC_Start(&_hadc1) != HAL_OK)
    {
        printf("Error starting ADC! \n");
        return;
    }
}

uint16_t dc_adc_get_value(void)
{
    /* Poll the ADC conversion */
    uint16_t reading = 0;
    uint16_t result = 0;
    if(HAL_ADC_PollForConversion(&_hadc1, 0xFF) != HAL_OK)
    {
        printf("Error polling for ADC conversion! \n");
    }
    else
    {
        /* Get the 12-bit reading */
        reading = HAL_ADC_GetValue(&_hadc1);
        // result = int()(reading/ADC_MAX)
    }
    return reading;
}


void dc_motor_set(float U)
{
    U = U <= -UMAX ? -UMAX : U >= UMAX ? UMAX : U;
    _voltage = U;

    if (U > 0.0f)
    {
        _dc_motor_set_direction(MOTOR_DIR_FWD);
    }
    else  if (U < 0.0f)
    {
        _dc_motor_set_direction(MOTOR_DIR_BCK);
    }
    else
    {
        _dc_motor_set_direction(MOTOR_DIR_OFF);
    }

    U = fabsf(U);

    float duty = U/UMAX;
    pulseWidth = (int)floor(PERIOD*duty);
    HAL_TIM_PWM_Stop(&_htim4, TIM_CHANNEL_2);
    // __HAL_TIM_SET_AUTORELOAD(&_htim4, CC);
    __HAL_TIM_SET_COMPARE(&_htim4, TIM_CHANNEL_2, pulseWidth);
    HAL_TIM_PWM_Start(&_htim4, TIM_CHANNEL_2);

}

float dc_motor_get(void)
{
    return _voltage;
}

void _dc_motor_set_direction(uint8_t dir)
{
    if(dir == MOTOR_DIR_FWD)
    {
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
    }
    else if (dir == MOTOR_DIR_BCK)
    {
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);
    }
}