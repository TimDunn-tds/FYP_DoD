#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"

#include "stm32f4xx_hal_pwr_ex.h"

#include "uart.h"
// #include "button.h"
// #include "led_rgb.h"
// #include "light.h"
// #include "potentiometer.h"
#include "encoder.h"
// #include "motor.h"
// #include "mpu6050.h"
// #include "kalman.h"

#include "heartbeat_task.h"
// #include "dimmer_task.h"
#include "cmd_task.h"
// #include "controller_task.h"
// #include "MPC_task.h"
// #include "sysID_task.h"

// #include "stepper_motor.h"
// #include "bluetooth.h"


#include "dc_motor.h"

static void SystemClock_Config(void);
static void Error_Handler(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    // Initialise scheduler
    osKernelInitialize();



    // Initialise hardware modules
    uart_init();
    // led_rgb_init();
    // button_init(); // this also starts the button task
    encoder_init(); 
    // pot_init();
    // motor_init();
    // MPU6050_init();

    dc_motor_init();
    dc_motor_set(0);

    dc_adc_init();


    
    // Initialise other modules
    // light_init();
    // kf_init();

    // Initialise task modules
    heartbeat_task_init();
    // dimmer_task_init();
    // controller_task_init();
    cmd_task_init();
    // MPC_task_init();
    // sysID_task_init();

    // Start scheduler
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */
    for(;/*ever*/;);
}


// STM32F411RE clock config
/*
 * System Clock Configuration:
 *           System Clock source     = PLL (HSI)
 *           SYSCLK(Hz)              = 100000000
 *           HCLK(Hz)                = 100000000
 *           AHB Prescaler           = 1
 *           APB1 Prescaler          = 2
 *           APB2 Prescaler          = 1
 *           HSI Frequency(Hz)       = 16000000
 *           PLL_M                   = 16
 *           PLL_N                   = 400
 *           PLL_P                   = 4
 *           PLL_Q                   = 7
 *           VDD(V)                  = 3.3
 *           Main regulator output voltage   = Scale2 mode
 *           Flash Latency(WS)       = 3
 */
static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /* Enable HSI Oscillator and activate PLL with HSI as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 0x10;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 400;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  // 50Mhz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }
}

static void Error_Handler(void)
{
    while(1)
    {
        /* Bury head in sand */
    }
}