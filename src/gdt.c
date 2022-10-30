//
// Created by root on 10/29/22.
//


#include "gdt.h"
#include "lib/int_ll32.h"
#include "cpu.h"
#include "multiboot.h"

void _work_to();

#define GDT_ENTRY 20

gdt_t _gdt[GDT_ENTRY];
u8 esp[1000];
tss_t def_tss;

void set_gdt_entry(u32 index, u32 base, u32 limit, u8 avl_attr, u8 type_dpl) {
    gdt_t *gdtDescriptor = &_gdt[index];
    gdtDescriptor->gdt_descriptor.base_address_1 = GET_BASE_L(base);
    gdtDescriptor->gdt_descriptor.base_address_2 = GET_BASE_M(base);
    gdtDescriptor->gdt_descriptor.base_address_3 = GET_BASE_H(base);
    gdtDescriptor->gdt_descriptor.offset_1 = GET_LIM_L(limit);
    gdtDescriptor->gdt_descriptor.attr_offset_2 = avl_attr << 4 |GET_LIM_H(limit);
    gdtDescriptor->gdt_descriptor.type_dpl = type_dpl;
}

void set_call_entry(u32 index, u32 address, u16 selector,u8 dpl) {
    gdt_t *callDescriptor = &_gdt[index];
    callDescriptor->call_descriptor.offset_1 = GET_CALL_BASE_L(address);
    callDescriptor->call_descriptor.offset_2 = GET_CALL_BASE_H(address);
    callDescriptor->call_descriptor.selector = selector;
    callDescriptor->call_descriptor.selector = selector;
    callDescriptor->call_descriptor.attr = 0B1100  | ((dpl & 0B11) << 5 ) |( 1 << 7);
}



void load_gdt() {
    gdt_index_t gdt_index;
    gdt_index._gdt_limit = sizeof(_gdt) ;
    gdt_index.gdt = &_gdt[0];
    cpu_lgdt((u32 *) &gdt_index);
}

void set_tss() {
    def_tss.cs = 0x1B;
    def_tss.ss = 0x23;
    def_tss.es = 0x23;
    def_tss.ds = 0x23;
    def_tss.fs = 0x23;
    def_tss.gs = 0x23;
    def_tss.ss = 0x23;
    def_tss.ss = 0x23;
    def_tss.eip = (u32)&_work_to;
    def_tss.esp = ((u32)&esp)+ sizeof(esp);
    def_tss.last_tss = 0x30;
}

void _init_gdt() {
    set_tss();
    set_gdt_entry(0, 0, 0, 0, 0);
    set_gdt_entry(1, 0, 0xfffff, GDT_DEF_ATTR, GDT_R0_CODE);
    set_gdt_entry(2, 0, 0xfffff, GDT_DEF_ATTR, GDT_R0_DATA);
    set_gdt_entry(3, 0, 0xfffff, GDT_DEF_ATTR, GDT_R3_CODE);
    set_gdt_entry(4, 0, 0xfffff, GDT_DEF_ATTR, GDT_R3_DATA);
    set_gdt_entry(5, (u32) &def_tss, sizeof(tss_t)-1, GDT_TSS_ATTR, TSS_R3_TYPE);
    set_gdt_entry(6, 0, sizeof(tss_t)-1, GDT_TSS_ATTR, TSS_R0_TYPE);
    load_gdt();

}