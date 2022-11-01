//
// Created by root on 10/31/22.
//

#include "lib/stdio.h"
int _task() {
    int a = 1;
    kprintf("runing _task\n");
    asm("iret" ::);
    return a + 3;
}
