#include <stdio.h>
#include "cmsis_os2.h"
#include "cmd_line_buffer.h"

#include "cmd_task.h"

CLB_CREATE_STATIC(clb, 80);

static osThreadId_t _cmdTaskHandle = NULL;

static void CmdTask_function(void *argument);

const osThreadAttr_t cmdTask_attr = {
  .priority = osPriorityHigh,             //Set initial thread priority to Normal   
  .stack_size = 1024                        //Set stack size to be larger to avoid crashes
};

static uint8_t _is_init = 0;

void cmd_task_init(void)
{
    clb_init(&clb);

    if (!_is_init)
    {
        _cmdTaskHandle = osThreadNew(CmdTask_function, NULL, &cmdTask_attr);
        _is_init = 1;
    }
}

void CmdTask_function(void *argument)
{
    printf("Hello World!\n");

    clb_init(&clb);
    
    while(1)
    {
        clb_process(&clb);
    }
}