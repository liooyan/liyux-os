//
// Created by 李延 on 2022/7/25.
//

#include "kernel/tty.h"
#include "arch/x86/boot/multiboot.h"

void _init_multiboot_info(multiboot_info_t* info){
    init_tty(info->framebuffer_width,info->framebuffer_height);
    for (int i = 0; i < 10000; ++i) {
        tty_put_str("test");
    }
}