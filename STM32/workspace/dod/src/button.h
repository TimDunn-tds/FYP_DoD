#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

void    button_init(void);
void 	button_deinit(void);
void 	button_debounce(void);
void    button_clear_pressed(void);
uint8_t button_get_pressed(void);
void    button_set_pressed(void);
uint8_t button_pop_pressed(void);
void 	button_isr(void);
void 	EXTI4_IRQHandler(void);


#endif