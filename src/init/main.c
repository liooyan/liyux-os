
//
// Created by 李延 on 2022/7/25.
//



#include "init/multiboot.h"
#include "lib/int_ll32.h"
#include "lib/tty.h"
#include "lib/stdio.h"
#include "mm/malloc.h"
#include "init/boot.h"
#include "arch/gdt.h"
#include "lib/elf32.h"
#include "arch/tss.h"


int _task1();
int _task2();

u32 _jump_to_boot();
multiboot_info_t *multiboot_info;
void _setup_init(multiboot_info_t *multiboot_info_parm, uint32_t heap_addr) {
    multiboot_info = multiboot_info_parm;
    //初始化屏幕信息
    _init_tty(multiboot_info->framebuffer_width, multiboot_info->framebuffer_height);
    //初始化堆
    heap_init(heap_addr, HEAP_SIZE);
    _init_gdt();
    kprintf("set cpu gdt end,will jump to boot\n");
    asm("ljmp %0,$0" ::"X"(BOOT_TSS_SELECTOR));
    //跳转到内核
}
void _start_kernel() {
    kprintf("this is _start_kernel\n");
    load_and_run_task((u32)_task1);
    load_and_run_task((u32)_task2);
}