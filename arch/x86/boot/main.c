
//
// Created by 李延 on 2022/7/25.
//

#include "multiboot.h"
#include "tty.h"
#include "stdio.h"

int _setup_init(multiboot_info_t* multiboot_info) {
    _init_tty(multiboot_info->framebuffer_width, multiboot_info->framebuffer_height);
    kprintf("The display initialization is complete");
    return 1;
}