#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#include <stdint.h>

void stepper_motor_init(void);
// void stepper_motor_gpio_init(void);
void stepper_motor_gpio_init_2(void);

void stepper_motor_step(uint16_t freq, uint16_t prsc);

void stepper_set_dir(int argc, char*argv[]);
void stepper_set_speed(int argc, char*argv[]);
void stepper_turn_off(int argc, char*argv[]);
void stepper_turn_on(int argc, char*argv[]);
float get_steps_per_rev(void);

void stepper_ctrl_speed(float dphi);

void stepper_backward(void);
void stepper_forward(void);
float stepper_ctralloc(float dphi);


#endif