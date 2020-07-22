#ifndef SYSID_TASK_H
#define SYSID_TASK_H

#include <stdint.h>

typedef enum {
    EXP_CART,
    EXP_PEND
} SYSID_EXPERIMENT_T;

void        sysID_task_init(void);
void        sysID_task_deinit(void);
void        sysID_task_resume(SYSID_EXPERIMENT_T experiment, uint32_t n_samples);
void        sysID_task_stop(void);
uint8_t     sysID_task_is_running(void);
void        sysID_cart_set_param(float amplitude, float frequency);

#endif