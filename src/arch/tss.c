//
// Created by root on 10/31/22.
//


#include "arch/tss.h"
#include "arch/gdt.h"
#include "lib/int_ll32.h"
#include "lib/stdio.h"


void _lcall(u32 selector);

static void set_tss(tss_t *base_tss, u16 data_selector, u16 code_selector, u16 ss_selector, u32 eip, u32 esp) {
    base_tss->cs = code_selector;
    base_tss->es = data_selector;
    base_tss->ds = data_selector;
    base_tss->fs = data_selector;
    base_tss->gs = data_selector;
    base_tss->ss = ss_selector;
    base_tss->eip = eip;
    base_tss->esp = esp;
}


void load_and_run_task(u32 start_addr) {

    u16 data_selector, code_selector, ss_selector, tss_selector;
    //准备tss
    tss_t tss;

    //准备栈
    u8 stack[2048];
    ss_selector = register_gdt_entry((u32) &stack, 1024, GDT_DEF_ATTR, GDT_R3_DATA) | 0B11;
    code_selector = register_gdt_entry(0, 0xfffff, GDT_DEF_ATTR, GDT_R3_CODE)| 0B11;
    data_selector = register_gdt_entry(0, 0xfffff, GDT_DEF_ATTR, GDT_R3_DATA)| 0B11;
     set_tss(&tss, data_selector, code_selector, ss_selector, start_addr, 2048);
    tss_selector = register_gdt_entry((u32) &tss, sizeof(tss_t) - 1, GDT_TSS_ATTR, TSS_R3_TYPE)| 0B11;
    _lcall((u32) tss_selector);
    kprintf("this task is end\n");
}




