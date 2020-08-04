#ifndef DC_MOTOR_H
#define DC_MOTOR_H

#include <stdint.h>

void dc_motor_init(void);
void dc_motor_set(float U);
void _dc_motor_set_direction(int8_t dir);
float dc_motor_get(void);
void dc_adc_init(void);
float dc_adc_get_value(void);
float dc_motor_get_duty(void);
float dc_adc_get_cs_value(void);
int8_t dc_motor_get_dir(void);



#endif
