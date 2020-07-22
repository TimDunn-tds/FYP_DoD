#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "button.h"
#include "led_rgb.h"
#include "light.h"
#include "potentiometer.h"
#include "encoder.h"
#include "dimmer_task.h"

static osThreadId_t _dimmer_task_thread_id;
static osThreadAttr_t _dimmerTask_attr = 
{
    .name = "dimmer",
    .priority = osPriorityLow,
};

static void _dimmer_task_update(void *argument);

static uint8_t _is_running = 0;
static uint8_t _is_init = 0;

void dimmer_task_init(void)
{
    if (!_is_init)
    {
        _dimmer_task_thread_id = osThreadNew(_dimmer_task_update, NULL, &_dimmerTask_attr);
        _is_running = 1;
        _is_init = 1;
    }
}

void dimmer_task_resume(void)
{
    if (!_is_running)
    {
        osThreadResume(_dimmer_task_thread_id);
        _is_running = 1;
    }
}

void dimmer_task_stop(void)
{
    if (_is_running)
    {
        osThreadSuspend(_dimmer_task_thread_id);
        _is_running = 0;
    }
}

uint8_t dimmer_task_is_running(void)
{
    return _is_running;
}

void _dimmer_task_update(void *argument)
{
    UNUSED(argument);

    /* Don't forget: You're here */ for (;/*ever*/;)
    {
        // Fixed LED sequence
        // for(int i = 8; i-->0;) { led_rgb_set_red(  UINT16_MAX*(i & 1));     osDelay(125); }
        // for(int i = 4; i-->0;) { led_rgb_set_green(UINT16_MAX*(i & 1));     osDelay(250); }
        // for(int i = 2; i-->0;) { led_rgb_set_blue( UINT16_MAX*(i & 1));     osDelay(500); }

        /* TODO: Implement dimmer logic
         *
         * 1) Read potentiometer and set light brightness
         * 2) Pop the encoder count and increment the light hue
         * 3) Pop the button state and toggle between 0% and 100% light saturation if pressed
         * 4) Set the RGB led channels from the light RGB channels
         * 5) Short osDelay
         *
         */


        // step 1: read pot and set light brightness
        uint16_t potValue = pot_get_value();


        light_set_brightness(potValue);



        // step 2: pop encoder count and increment light hue
        float encoderValue = encoder_pop_count();
        float hue = encoderValue *(360/96.0)*LIGHT_HUE_DEGREE;
        light_incr_hue(hue);


        // step 3: pop button and toggle light saturation
        uint16_t wasPressed = button_pop_pressed();
        if(wasPressed==1)
        {
            if(light_get_saturation() < LIGHT_MAX_SATURATION)
            {
                light_set_saturation(LIGHT_MAX_SATURATION);
            }
            else
            {
                light_set_saturation(LIGHT_MIN_SATURATION);
            }
        }


        // step 4: set the RGB led channels from the light RGB channels
        led_rgb_set_blue(light_get_blue());
        led_rgb_set_green(light_get_green());
        led_rgb_set_red(light_get_red());

        // step 5: osdelay
        osDelay(50);




    }
}

void dimmer_task_deinit(void)
{
    _is_init = 0;
    _is_running = 0;
}