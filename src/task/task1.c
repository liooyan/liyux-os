//
// Created by root on 10/31/22.
//

#include "arch/tss.h"
int _task2();
int _task1() {
    int a = 1;

    load_and_run_task((u32)_task2);
    asm("iret" ::);
    return a + 3;
}
