//
// Created by Administrator on 2022/7/27.
//

#ifndef LIYUX_OS_INTERRUPT_H
#define LIYUX_OS_INTERRUPT_H

#include <kernel/interrupts.h>



#define ISR(iv) void _asm_isr##iv();

ISR(0)
ISR(81)

void isr0();
void keyword_event();

#endif //LIYUX_OS_INTERRUPT_H