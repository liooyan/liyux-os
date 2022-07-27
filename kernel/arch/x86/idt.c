//
// Created by Administrator on 2022/7/27.
//
#include <stdint.h>
#include <kernel/interrupt.h>
#include <arch/x86/idt.h>
#include <arch/x86/gdt.h>

#define IDT_ENTRY 256


uint64_t _idt[IDT_ENTRY];
uint16_t _idt_limit = sizeof(_idt) - 1;
int_subscriber _idt_function[IDT_ENTRY];

static void set_idt_entry(uint32_t vector,
                          uint16_t seg_selector,
                          void (*isr)(),
                          void (*function)(),
                          uint8_t dpl) {
    uint32_t offset = (uint32_t) isr;
    _idt[vector] = (offset & 0xffff0000) | IDT_ATTR(dpl);
    _idt[vector] <<= 32;
    _idt[vector] |= (seg_selector << 16) | (offset & 0x0000ffff);
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
}