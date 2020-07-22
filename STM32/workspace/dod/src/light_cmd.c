#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "led_rgb.h"
#include "dimmer_task.h"
#include "light.h"
#include "light_cmd.h"

void light_cmd(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Current LED hue = %.0fdeg, sat = %.0f%%, val = %.0f%%\n",
            (float)light_get_hue()/LIGHT_HUE_DEGREE,
            100.0*light_get_saturation()/UINT16_MAX,
            100.0*light_get_brightness()/UINT16_MAX);
    }
    else if (argc == 4)
    {
        if (dimmer_task_is_running())
        {
            dimmer_task_stop();
            printf("Dimmer has stopped\n");
        }
        
        float hue = atof(argv[1]);
        float sat = atof(argv[2]);
        float val = atof(argv[3]);

        // Input sanitisation
        if (hue<0)
        {
            hue = 360*LIGHT_HUE_DEGREE + hue;
            hue = fmodf(hue,360);
        }
        else 
        {
            hue=fmodf(hue,360);
        }
        sat = sat <= 0 ? 0 : sat >= 100 ? 100 : sat;
        val = val <= 0 ? 0 : val >= 100 ? 100 : val;

        // Set HSV values
        light_set_hue((uint16_t)roundf(hue*LIGHT_HUE_DEGREE));
        light_set_saturation((uint16_t)roundf(sat*UINT16_MAX/100));
        light_set_brightness((uint16_t)roundf(val*UINT16_MAX/100));

        // Update LED RGB channels
        led_rgb_set_red(light_get_red());
        led_rgb_set_green(light_get_green());
        led_rgb_set_blue(light_get_blue());

        printf("Set LED hue = %.0fdeg, sat = %.0f%%, val = %.0f%%\n",
            (float)light_get_hue()/LIGHT_HUE_DEGREE,
            100.0*light_get_saturation()/UINT16_MAX,
            100.0*light_get_brightness()/UINT16_MAX);
    }
    else
    {
        printf("%s: invalid argument \"%s\", syntax is: %s [<hue> <saturation> <brightness>]\n", argv[0], argv[1], argv[0]);
    }
}