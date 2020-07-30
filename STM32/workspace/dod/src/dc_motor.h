#ifndef DC_MOTOR_H
#define DC_MOTOR_H

#include <stdint.h>

void dc_motor_init(void);
void dc_motor_set(float U);
void _dc_motor_set_direction(uint8_t dir);
float dc_motor_get(void);
void dc_adc_init(void);
uint16_t dc_adc_get_value(void);



#endif
