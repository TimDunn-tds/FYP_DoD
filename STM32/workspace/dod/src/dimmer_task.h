#ifndef DIMMER_TASK_H
#define DIMMER_TASK_H

#include <stdint.h>

void        dimmer_task_init(void);
void        dimmer_task_deinit(void);
void        dimmer_task_resume(void);
void        dimmer_task_stop(void);
uint8_t     dimmer_task_is_running(void);

#endif