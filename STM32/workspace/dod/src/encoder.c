#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "encoder.h"

static void _encoder_enable_interrupts(void);
static void _encoder_disable_interrupts(void);

static uint8_t _is_init = 0;
static int32_t _count = 0;

void encoder_init(void)
{
    // Set default count
    _count = 0; 

    if (!_is_init)  // Guards against multiple initialisations
    {
        // PC0 ENCA
        // PC1 ENCB
        /* DONE:    Configure and initialise PC0 and PC1 with
                    - External interrupts on the rising and falling edges
                    - Pullup resistors enabled                  */

        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
        GPIO_InitStructure.Pull = GPIO_PULLUP;
        /* DONE: Enable GPIOC clock */
        __HAL_RCC_GPIOC_CLK_ENABLE();
        /* DONE: Initialise GPIOC */
        HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
        /* DONE: Set the priorities of the EXTI0 and EXTI1 interrupts to
                    - Preempt: 0x02, 
                    - Sub: 0x00 */
        HAL_NVIC_SetPriority(EXTI0_IRQn, 0x02, 0x00);
        HAL_NVIC_SetPriority(EXTI1_IRQn, 0x02, 0x00);
        /* DONE: Enable the EXTI0 and EXTI1 IRQs using the NVIC */
        HAL_NVIC_EnableIRQ(EXTI0_IRQn);
        HAL_NVIC_EnableIRQ(EXTI1_IRQn);

        // HAL_ADC_Init();
        _is_init = 1;
    }
}

void encoder_edge_B_isr(void)
{
    // DONE: Implement B edge logic to increment or decrement _count
    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)) {
        _count++;
    }
    else {
        _count--;
    }


}

void encoder_edge_A_isr(void)
{
    // DONE: Implement A edge logic to increment or decrement _count
    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)) {
        _count--;
    }
    else {
        _count++;
    }    

}

void encoder_set_count(int32_t count)
{
    // Atomically set _count
    _encoder_disable_interrupts();
    _count = count;
    _encoder_enable_interrupts();
}

int32_t encoder_get_count(void)
{
    // Atomically read _count
    _encoder_disable_interrupts();
    int32_t count = _count;
    _encoder_enable_interrupts();
    return count;
}

int32_t encoder_pop_count(void)
{
    // Atomically read and reset _count
    _encoder_disable_interrupts();
    int32_t count = _count;
    _count = 0;
    _encoder_enable_interrupts();
    return count;
}

void _encoder_enable_interrupts(void)
{
    /* DONE?: Enable the EXTI0 and EXTI1 IRQs using the NVIC */
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

}

void _encoder_disable_interrupts(void)
{
    /* DONE?: Disable the EXTI0 and EXTI1 IRQs using the NVIC */
    HAL_NVIC_DisableIRQ(EXTI0_IRQn);
    HAL_NVIC_DisableIRQ(EXTI1_IRQn);
}

void EXTI0_IRQHandler(void)
{
    /* DONE: Call the encoder edge A isr */
    encoder_edge_A_isr();
    /* DONE: Call the HAL GPIO EXTI IRQ Handler and specify the GPIO pin */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void EXTI1_IRQHandler(void)
{
    /* DONE: Call the encoder edge B isr */
    encoder_edge_B_isr();
    /* DONE: Call the HAL GPIO EXTI IRQ Handler and specify the GPIO pin */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}