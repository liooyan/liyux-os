//
// Created by lioyan.cn on 2022/7/26.
//
#include <arch/x86/gdt.h>
#include <stdint.h>

#define GDT_ENTRY 6

uint64_t _gdt[GDT_ENTRY];
uint16_t _gdt_limit = sizeof(_gdt) - 1;


static void set_gdt_entry(uint32_t index, uint32_t base, uint32_t limit, uint32_t flags) {
    _gdt[index] = SET_BASE_H(base) | flags | SET_LIM_H(limit) | SET_BASE_M(base);
    _gdt[index] <<= 32;
    _gdt[index] |= SET_BASE_L(base) | SET_LIM_L(limit);
}

static void set_r0_code_gdt(uint32_t index, uint32_t base, uint32_t limit) {
    set_gdt_entry(index,base,limit,GDT_R0_CODE);
}

static void set_r0_data_gdt(uint32_t index, uint32_t base, uint32_t limit) {
    set_gdt_entry(index,base,limit,GDT_R0_DATA);
}




void _init_gdt(){
    set_gdt_entry(0, 0, 0, 0);
    set_r0_code_gdt(1, 0, 0xfffff);
    set_r0_data_gdt(2, 0, 0xfffff);
}