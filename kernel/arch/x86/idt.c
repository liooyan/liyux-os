//
// Created by Administrator on 2022/7/27.
//
#include <stdint.h>
#include <kernel/interrupt.h>
#include <arch/x86/idt.h>
#include <arch/x86/gdt.h>
#include <arch/x86/rlevel.h>
#include "arch/x86/cpu.h"

#define IDT_ENTRY 256


idt_descriptor_t _idt[IDT_ENTRY];
uint16_t _idt_limit = sizeof(_idt) - 1;
int_subscriber _idt_function[IDT_ENTRY];




static void init_idt_descriptor(idt_descriptor_t *idt, uint16_t selector,uint32_t offset,uint8_t dpl ){
    idt->selector = selector;
    idt->offset_1 = 0xffff&offset;
    idt->offset_2 = offset>>16;
    idt->attr = (1 << 7) | ((dpl & 3) << 5) | (1 << 3) | (3 << 1);
}

static void set_idt_entry(uint32_t vector,
                          uint16_t seg_selector,
                          void (*isr)(),
                          void (*function)(),
                          uint8_t dpl) {
    init_idt_descriptor(&_idt[vector],seg_selector,isr,R0);
    _idt_function[vector]  = function;
}

/**
 * 安装idt， selector使用0x08，特权等级为0
 * @param vector
 * @param isr
 */
static void set_idt_dlp0_global(uint32_t vector, void (*isr)(),void (*function)()) {
    set_idt_entry(vector, GDT_SELECTOR_CODE_GLOBAL, isr, function,R0);
}

void _init_idt() {
    set_idt_dlp0_global(FAULT_DIVISION_ERROR, _asm_isr0,isr0);
    set_idt_dlp0_global(TRIGGER_KEYWORD_EVENT, _asm_isr81,keyword_event);

    idt_index_t index = {_idt_limit,_idt};
    cpu_lidt((uint32_t *) &index);


}