//
// Created by root on 11/2/22.
//

#include "kernel/task.h"
#include "arch/ldt.h"
#include "arch/gdt.h"
#include "lib/int_ll32.h"


#include "arch/ldt.h"
#include "lib/int_ll32.h"
#include "arch/cpu.h"
#include "arch/tss.h"
#include "kernel/task.h"
#include "mm/malloc.h"

void _task_lcall(u32 selector);
void do_task_run( void (*start)()) ;
static void set_tss(tss_t *base_tss, u16 data_selector, u16 code_selector, u16 ss_selector, u32 eip, u32 esp,u16 ldt) {
    base_tss->cs = code_selector;
    base_tss->es = data_selector;
    base_tss->ds = data_selector;
    base_tss->fs = data_selector;
    base_tss->gs = data_selector;
    base_tss->ss = ss_selector;
    base_tss->eip = eip;
    base_tss->esp = esp;
    base_tss->ldt = ldt;
    base_tss->eflags = 1<< 9;
}

task_t *creat_def_task(u32 start_add) {
    task_t *task = (task_t *)malloc_4k(sizeof(task_t));
    task->ldt_size = DEF_LDT_SIZE;
    task->stack_size = TASK_STACK_SIZE;

    u32 ldt_index = register_ldt_in_gdt(task->ldt, task->ldt_size);
    task->ldt_index = ldt_index;

    u16 ss_selector = register_ldt_entry(&task->ldt[0],task->ldt_size,(u32) &task->stack, task->stack_size, GDT_DEF_ATTR, GDT_R3_DATA);
    u16 code_selector = register_ldt_entry(&task->ldt[0],task->ldt_size, 0, 0xfffff, GDT_DEF_ATTR, GDT_R3_CODE);
    u16 data_selector = register_ldt_entry(&task->ldt[0], task->ldt_size,0, 0xfffff, GDT_DEF_ATTR, GDT_R3_DATA);
    set_tss(&task->tss, data_selector, code_selector, ss_selector, (u32)do_task_run, task->stack_size-8,task->ldt_index);

    u32 *stack_end = (u32 *)((u32)(&task->stack)+task->stack_size-4);
    *stack_end = start_add;


    //ss0
    u16 ss_0_selector =  register_ldt_entry(&task->ldt[0],task->ldt_size,(u32) &task->stack_0, task->stack_0_size, GDT_DEF_ATTR, GDT_R0_DATA);
    task->tss.ss0 = ss_0_selector;
    task->tss.esp0 = task->stack_0_size-4;

    task->tss_selector = register_gdt_entry( (u32) &task->tss, sizeof(tss_t)-1, GDT_TSS_ATTR, TSS_R0_TYPE);
    task->task_selector = register_task_entry( task->tss_selector);

    task->task_start = start_add;

    return task;

}


void task_run(task_t *task) {
    _task_lcall((u32) task->task_selector);
}
int _task();
void do_task_run( void (*start)()) {
    start();
    asm("iret" ::);
}