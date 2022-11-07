//
// Created by root on 11/7/22.
//

#ifndef LIYUX_OS_IDT_H
#define LIYUX_OS_IDT_H

#include "lib/int_ll32.h"

typedef struct  {
    u16 offset_1;        // 低16位偏移地址
    u16 selector;        // 段选择子
    u8 zero;            // 固定为0
    u8 attr; // gate type, dpl, and p fields
    u16 offset_2;        // 高16位偏移地址
} idt_descriptor_t;

typedef struct  {
    u16 _idt_limit;
    idt_descriptor_t *idt;
} __attribute__((packed)) idt_index_t ;

void _init_idt();

typedef void (*int_subscriber)();

#endif //LIYUX_OS_IDT_H
