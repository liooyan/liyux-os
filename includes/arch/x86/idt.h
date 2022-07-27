//
// Created by Administrator on 2022/7/27.
//

#ifndef LIYUX_OS_IDT_H
#define LIYUX_OS_IDT_H


#define IDT_ATTR(dpl)    ( (0x47<<9) | ((dpl & 3) << 13) | (1 << 15))

void _init_idt();
typedef void (*int_subscriber)();

#endif //LIYUX_OS_IDT_H
