#include "stm32f4xx_hal.h"
#include "motor.h"
#include "math.h"

#define     UMAX (12.0f)

#define     MOTOR_DIR_FWD   1
#define     MOTOR_DIR_BCK   2

DAC_HandleTypeDef _dacHandle;
float _Voltage = 0.0f;

static void _motor_dac_init(void);
static void _motor_gpio_init(void);
static void _motor_set_direction(uint8_t dir);

void motor_init(void)
{
    _motor_gpio_init();
    _motor_dac_init();
}

void _motor_dac_init(void)
{
    _dacHandle.Instance = DAC1;

    if (HAL_DAC_Init(&_dacHandle) != HAL_OK)
    {
      /* Initialization Error */
      printf("Error initialising DAC!\n");
    }

    /*##-2- Configure DAC channel1 #############################################*/
    DAC_ChannelConfTypeDef sConfig;
    sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;

    if (HAL_DAC_ConfigChannel(&_dacHandle, &sConfig, DAC_CHANNEL_1) != HAL_OK)
    {
      /* Channel configuration Error */
      printf("Error initialising DAC!\n");
    }

    /*##-3- Set DAC Channel1 DHR register ######################################*/
    if (HAL_DAC_SetValue(&_dacHandle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0x00) != HAL_OK)
    {
      /* Setting value Error */
      printf("Error initialising DAC!\n");
    }

    /*##-4- Enable DAC Channel1 ################################################*/
    if (HAL_DAC_Start(&_dacHandle, DAC_CHANNEL_1) != HAL_OK)
    {
      /* Start Error */
      printf("Error initialising DAC!\n");
    }

}

void _motor_gpio_init(void)
{
    /* Initialise DIR pins */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.Pin      = GPIO_PIN_7;
    GPIO_InitStructure.Mode     = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull     = GPIO_PULLUP;
    GPIO_InitStructure.Speed    = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Initialise Motor DAC GPIO */
    // PA4 = DAC OUT 1

    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO clock ****************************************/
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /* DAC Periph clock enable */
    __HAL_RCC_DAC_CLK_ENABLE();
    /*##-2- Configure peripheral GPIO ##########################################*/
    /* DAC Channel1 GPIO pin configuration */
    GPIO_InitStructure.Pin = GPIO_PIN_4;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void motor_set(float U)
{
    U = U <= -UMAX ? -UMAX : U >= UMAX ? UMAX : U;

    _Voltage = U;

    if (U > 0.0f)
    {
        _motor_set_direction(MOTOR_DIR_FWD);
    }
    else // if (U < 0.0f)
    {
        _motor_set_direction(MOTOR_DIR_BCK);
    }
    U = fabsf(U);

    float duty = U/UMAX;
    uint16_t motor_u_DAC = floor(duty*4095.0f);

    /*##-3- Set DAC Channel1 DHR register ######################################*/
    if (HAL_DAC_SetValue(&_dacHandle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, motor_u_DAC) != HAL_OK)
    {
      /* Setting value Error */
      printf("Error initialising DAC!\n");
    }
}

float motor_get(void)
{
    return _Voltage;
}

void _motor_set_direction(uint8_t dir)
{
    if(dir == MOTOR_DIR_FWD)
    {
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
    }
}