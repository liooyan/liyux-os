//
// Created by root on 10/29/22.
//


#include "gdt.h"
#include "lib/int_ll32.h"
#include "cpu.h"


#define GDT_ENTRY 6

gdt_descriptor_t _gdt[GDT_ENTRY];

void set_gdt_entry(u32 index, u32 base, u32 limit, u8 avl_attr, u8 type_dpl) {
    gdt_descriptor_t *gdtDescriptor = &_gdt[index];
    gdtDescriptor->base_address_1 = GET_BASE_L(base);
    gdtDescriptor->base_address_2 = GET_BASE_M(base);
    gdtDescriptor->base_address_3 = GET_BASE_H(base);
    gdtDescriptor->offset_1 = GET_LIM_L(limit);
    gdtDescriptor->attr_offset_2 = avl_attr << 4 |GET_LIM_H(limit);
    gdtDescriptor->type_dpl = type_dpl;
}

void load_gdt() {
    gdt_index_t gdt_index;
    gdt_index._gdt_limit = sizeof(_gdt) ;
    gdt_index.gdt = &_gdt[0];
    cpu_lgdt((u32 *) &gdt_index);
}

void _init_gdt() {
    set_gdt_entry(0, 0, 0, 0, 0);
    set_gdt_entry(1, 0, 0xfffff, GDT_DEF_ATTR, GDT_R0_CODE);
    set_gdt_entry(2, 0, 0xfffff, GDT_DEF_ATTR, GDT_R0_DATA);
    load_gdt();

}