//
// Created by root on 10/31/22.
//

#include "lib/stdio.h"
#include "lib/int_ll32.h"
u32 by_zero(u32 num);
int _task() {
    by_zero(0);
    int a = 1;
    kprintf("runing _task\n");
    asm("iret" ::);
    return a + 3;
}

