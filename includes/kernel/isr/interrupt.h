//
// Created by Administrator on 2022/7/27.
//

#ifndef LIYUX_OS_INTERRUPT_H
#define LIYUX_OS_INTERRUPT_H

#include <kernel/isr/interrupts.h>



#define ISR(iv) void _asm_isr##iv();


ISR(0)


void isr0();

#endif //LIYUX_OS_INTERRUPT_H