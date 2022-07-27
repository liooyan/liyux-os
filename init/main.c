//
// Created by 李延 on 2022/7/25.
//

#include "kernel/tty.h"
#include <arch/x86/idt.h>
#include "arch/x86/boot/multiboot.h"
#include <arch/x86/system.h>

int _kernel_init(int a ,int b) {
    _init_tty(multiboot_info->framebuffer_width, multiboot_info->framebuffer_height);
    sti();
    for (int i = 0; i < 10000; ++i) {
        int f = a/b;
        tty_put_str("test\n");
        return f;
    }
}