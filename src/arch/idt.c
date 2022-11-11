//
// Created by Administrator on 2022/7/27.
//
#include <arch/idt.h>
#include "arch/rlevel.h"
#include "arch/gdt.h"
#include "arch/cpu.h"
#include "kernel/isr/interrupts.h"
#include "kernel/isr/interrupt.h"


#define IDT_ENTRY 256


idt_descriptor_t _idt[IDT_ENTRY];
u16 _idt_limit = sizeof(_idt) - 1;
int_subscriber _idt_function[IDT_ENTRY];




static void init_idt_descriptor(idt_descriptor_t *idt, u16 selector,u32 offset,u8 dpl ){
    idt->selector = selector;
    idt->offset_1 = 0xffff&offset;
    idt->offset_2 = offset>>16;
    idt->attr = (1 << 7) | ((dpl & 3) << 5) | (1 << 3) | (3 << 1);
}

/**
 * 安装idt， selector使用0x08，特权等级为0
 * @param vector
 * @param isr
 */
static void set_idt_dlp0_global(u32 vector, void (*isr)(),void (*function)()) {
    init_idt_descriptor(&_idt[vector],0x08,isr,R0);
    _idt_function[vector]  = function;
}

void _init_idt() {
    set_idt_dlp0_global(FAULT_DIVISION_ERROR, _asm_isr0,isr0);
    set_idt_dlp0_global(TIMER_DIVISION, _asm_isr60,isr_time);
    idt_index_t index = {_idt_limit,_idt};
    cpu_lidt((u32 *) &index);


}