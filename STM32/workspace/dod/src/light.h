#ifndef LIGHT_H
#define LIGHT_H

#include <stdint.h>

enum {
    LIGHT_MIN_BRIGHTNESS        = 0,
    LIGHT_MAX_BRIGHTNESS        = UINT16_MAX,
    LIGHT_DEFAULT_BRIGHTNESS    = (LIGHT_MIN_BRIGHTNESS + LIGHT_MAX_BRIGHTNESS)/2,
    LIGHT_MIN_SATURATION        = 0,
    LIGHT_MAX_SATURATION        = UINT16_MAX,
    LIGHT_DEFAULT_SATURATION    = UINT16_MAX,
    LIGHT_HUE_DEGREE            = 128,  // Number of hue levels corresponding to 1 degree
    LIGHT_DEFAULT_HUE           = 0*LIGHT_HUE_DEGREE,
};

_Static_assert(LIGHT_HUE_DEGREE % 4 == 0, "LIGHT_HUE_DEGREE must be divisible by 4");
_Static_assert(360*LIGHT_HUE_DEGREE - 1 <= UINT16_MAX, "Can't store 360 degree range of hue within a uint16_t");

void        light_init(void);

void        light_set_brightness(uint16_t);
uint16_t    light_get_brightness(void);

void        light_set_saturation(uint16_t);
uint16_t    light_get_saturation(void);

void        light_set_hue(uint16_t);
void        light_incr_hue(int16_t);
uint16_t    light_get_hue(void);

uint16_t    light_get_red(void);
uint16_t    light_get_green(void);
uint16_t    light_get_blue(void);

#endif