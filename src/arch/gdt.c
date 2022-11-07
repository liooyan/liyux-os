//
// Created by root on 10/29/22.
//


#include "arch/gdt.h"
#include "lib/int_ll32.h"
#include "arch/cpu.h"
#include "arch/tss.h"

#define GDT_ENTRY 20

gdt_t _gdt[GDT_ENTRY];

u32 gdt_use_num = 0;

tss_t base_tss;

void _start_kernel();

//正式使用的栈空间
u32 stack[1024];


static void set_gdt_entry(u32 index, u32 base, u32 limit, u8 avl_attr, u8 type_dpl) {
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


u32 register_task_entry( u16 tss_selector) {
    gdt_use_num++;
    gdt_t *gdtDescriptor = &_gdt[gdt_use_num];
    gdtDescriptor->task_descriptor.selector = tss_selector;
    gdtDescriptor->task_descriptor.attr = TASK_R0_TYPE;
    return  gdt_use_num << 3;
}


static void load_gdt() {
    gdt_index_t gdt_index;
    gdt_index._gdt_limit = sizeof(_gdt) ;
    gdt_index.gdt = &_gdt[0];
    cpu_lgdt((u32 *) &gdt_index);
}

static void set_boot_tss() {
    base_tss.cs = BOOT_GDT_CODE;
    base_tss.ss = BOOT_GDT_DATA;
    base_tss.es = BOOT_GDT_DATA;
    base_tss.ds = BOOT_GDT_DATA;
    base_tss.fs = BOOT_GDT_DATA;
    base_tss.gs = BOOT_GDT_DATA;
    base_tss.ss = BOOT_GDT_DATA;
    base_tss.ss = BOOT_GDT_DATA;
    base_tss.eip = (u32)&_start_kernel;
    base_tss.esp = (u32)stack+ sizeof(stack);
}

void _init_gdt() {
    set_boot_tss();
    set_gdt_entry(0, 0, 0, 0, 0);
    set_gdt_entry(1, 0, 0xfffff, GDT_DEF_ATTR, GDT_R0_CODE);
    set_gdt_entry(2, 0, 0xfffff, GDT_DEF_ATTR, GDT_R0_DATA);
    set_gdt_entry(3, (u32) &base_tss, sizeof(tss_t)-1, GDT_TSS_ATTR, TSS_R0_TYPE);
    gdt_use_num = 3;
    load_gdt();
}

u32 register_gdt_entry(u32 base, u32 limit, u8 avl_attr, u8 type_dpl) {
    gdt_use_num++;
    set_gdt_entry(gdt_use_num,base,limit,avl_attr,type_dpl);
    return gdt_use_num<<3;
}