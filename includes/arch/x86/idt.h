//
// Created by Administrator on 2022/7/27.
//

#ifndef LIYUX_OS_IDT_H
#define LIYUX_OS_IDT_H



typedef struct  {
    uint16_t offset_1;        // 低16位偏移地址
    uint16_t selector;        // 段选择子
    uint8_t zero;            // 固定为0
    uint8_t attr; // gate type, dpl, and p fields
    uint16_t offset_2;        // 高16位偏移地址
} idt_descriptor_t;

typedef struct  {
    uint16_t _idt_limit;
    idt_descriptor_t *idt;
} __attribute__((packed)) idt_index_t ;

void _init_idt();

typedef void (*int_subscriber)();

#endif //LIYUX_OS_IDT_H
