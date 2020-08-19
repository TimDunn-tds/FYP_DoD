#ifndef DC_MOTOR_TASK_H
#define DC_MOTOR_TASK_H

#include <stdint.h>

void dc_motor_current_task_init(void);
void dc_motor_current_task_resume(void);
void dc_motor_current_task_stop(void);
uint8_t dc_motor_current_task_is_running(void);
void dc_motor_current_task_deinit(void);






#endif