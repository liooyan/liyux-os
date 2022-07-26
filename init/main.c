//
// Created by 李延 on 2022/7/25.
//

#include "kernel/tty.h"
#include <arch/x86/gdt.h>
#include "arch/x86/boot/multiboot.h"

void _init_multiboot_info(){
    init_tty(multiboot_info->framebuffer_width,multiboot_info->framebuffer_height);
    for (int i = 0; i < 10000; ++i) {
        tty_put_str("test\n");
    }
}