#ifndef MPC_TASK_H
#define MPC_TASK_H

#include <stdint.h>

void        MPC_task_init(void);
void        MPC_task_deinit(void);
void        MPC_task_resume(void);
void        MPC_task_stop(void);
uint8_t     MPC_task_is_running(void);

#endif