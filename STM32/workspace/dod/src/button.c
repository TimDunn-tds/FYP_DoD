#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"

#include "button.h"
 
#define BUTTON_SIGNAL 0x0001
static osEventFlagsAttr_t _buttonEventAttr = { .name = "button" };
static osThreadAttr_t _buttonTask_attr = 
{
    .name = "button",
    .priority = osPriorityHigh //Set initial thread priority
};

static uint8_t _was_pressed = 0;    // Sticky flag

osThreadId_t        _button_thread_id;
osEventFlagsId_t    _button_evt_id;

void _button_thread_func(void *argument);

static uint8_t _is_init = 0;

void button_init(void)
{
    _was_pressed = 0;

    if (!_is_init)
    {
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Pin = GPIO_PIN_4;
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        __HAL_RCC_GPIOC_CLK_ENABLE();
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
        HAL_NVIC_SetPriority(EXTI4_IRQn, 0x01, 0x00);
        HAL_NVIC_EnableIRQ(EXTI4_IRQn);

        _button_thread_id = osThreadNew(_button_thread_func, NULL, &_buttonTask_attr);
        _button_evt_id = osEventFlagsNew(&_buttonEventAttr);

        _is_init = 1;
    }
}

void button_deinit(void)
{
    _is_init = 0;
    _was_pressed = 0;
}

void _button_thread_func(void *argument)
{
    while(1)
    {
        button_debounce();
    }
}

void button_debounce(void)
{
    typedef enum {
        BUTTON_LOW,
        BUTTON_HIGH,
    } BUTTON_STATE_T;
    
    static BUTTON_STATE_T state = BUTTON_LOW;

    switch (state)
    {
    case BUTTON_LOW:
        osEventFlagsWait(_button_evt_id, BUTTON_SIGNAL, osFlagsWaitAny, osWaitForever); // Wait forever for the event bit BUTTON_SIGNAL
        osDelay(30);    // Rising edge button debounce
        if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) == GPIO_PIN_SET)
        {
            _was_pressed = 1;
            state = BUTTON_HIGH;
        }
        osEventFlagsClear(_button_evt_id, BUTTON_SIGNAL);
        break;
    case BUTTON_HIGH:
        osEventFlagsWait(_button_evt_id, BUTTON_SIGNAL, osFlagsWaitAny, osWaitForever); // Wait forever for the event bit BUTTON_SIGNAL
        osDelay(50);    // Falling edge button debounce
        if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) == GPIO_PIN_RESET)
        {
            state = BUTTON_LOW;
        }
        osEventFlagsClear(_button_evt_id, BUTTON_SIGNAL); // Clear additional pending events during delay
        break;
    default:
        // Something went horribly wrong
        break;
    }
}

void button_clear_pressed(void)
{
    _was_pressed = 0;
}

uint8_t button_get_pressed(void)
{
    return _was_pressed;
}

void button_set_pressed(void)
{
    _was_pressed = 1;
}

uint8_t button_pop_pressed(void)
{
    uint8_t was_pressed = _was_pressed;
    _was_pressed = 0;
    return was_pressed;
}

void button_isr(void)
{
    osEventFlagsSet(_button_evt_id, BUTTON_SIGNAL);
}

void EXTI4_IRQHandler(void)
{
    button_isr();
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}