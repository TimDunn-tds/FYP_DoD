#ifndef CONTROLLER_TASK_H
#define CONTROLLER_TASK_H

#include <stdint.h>

void        controller_task_init(void);
void        controller_task_deinit(void);
void        controller_task_resume(void);
void        controller_task_stop(void);
uint8_t     controller_task_is_running(void);

#endif