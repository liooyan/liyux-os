//
// Created by root on 11/2/22.
//

#ifndef LIYUX_OS_TASK_H
#define LIYUX_OS_TASK_H


#include "arch/ldt.h"
#include "arch/tss.h"

#define DEF_LDT_SIZE 256
#define TASK_STACK_SIZE 1024
typedef struct {
    ldt_t ldt[DEF_LDT_SIZE];
    u16 ldt_index;
    u16 ldt_size;
    tss_t tss;
    u16 tss_selector;
    u8 stack[TASK_STACK_SIZE];
    u32 stack_size;

} task_t;

task_t *creat_def_task(u32 start_add);

void task_run(task_t *task);
#endif //LIYUX_OS_TASK_H
