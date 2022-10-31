//
// Created by root on 10/31/22.
//

#ifndef LIYUX_OS_TSS_H
#define LIYUX_OS_TSS_H
#include "lib/int_ll32.h"
#include "lib/elf32.h"

typedef struct  {
    u16 last_tss;
    u16 keep_1;
    u32 esp0;
    u16 ss0;
    u16 keep_2;
    u32 esp1;
    u16 ss1;
    u16 keep_3;
    u32 esp2;
    u16 ss2;
    u16 keep_4;
    u32 cr3;

    u32 eip;
    u32 eflags;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;
    u16 es;
    u16 keep_5;
    u16 cs;
    u16 keep_6;
    u16 ss;
    u16 keep_7;
    u16 ds;
    u16 keep_8;
    u16 fs;
    u16 keep_9;
    u16 gs;
    u16 keep_10;
    u16 ldt;
    u16 keep_11;
    u16 keep_12;
    u16 io_map;
} tss_t;


void  load_and_run_task(u32 start_addr);
void  load_and_run_task_2(u32 start_addr);

#endif //LIYUX_OS_TSS_H
