//
// Created by Administrator on 2022/7/27.
//

#ifndef LIYUX_OS_INTERRUPT_H
#define LIYUX_OS_INTERRUPT_H

#include <kernel/interrupts.h>



#define ISR(iv) void _asm_isr##iv();

ISR(0)
ISR(61)
ISR(60)

void isr0();
void isr60();
void isr61();

#endif //LIYUX_OS_INTERRUPT_H