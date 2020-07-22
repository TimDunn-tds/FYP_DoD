#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

void    motor_init(void);
void    motor_set(float);
float   motor_get(void);

#endif