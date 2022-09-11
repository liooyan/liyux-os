
//
// Created by 李延 on 2022/7/25.
//

#include "multiboot.h"
#include "tty.h"
#include "elf64.h"
#include "boot.h"
#include "malloc.h"
#include "kmem.h"
#include "stdio.h"

void _jump_to_64(boot_params_t *hold_mem);

void _setup_init(multiboot_info_t *multiboot_info, uint32_t heap_addr) {
    //初始化屏幕信息
    _init_tty(multiboot_info->framebuffer_width, multiboot_info->framebuffer_height);
    //初始化堆
    heap_init(heap_addr, HEAP_SIZE);
    boot_params_t  *boot_params = malloc(sizeof(boot_params_t));
    //加载64位内核代码
     loading_kernel(multiboot_info,boot_params);

    //设在64位相关内容
    cut64( boot_params);
    kprintf("set cpu 64 end,will jump to boot");
    _jump_to_64(boot_params);
    //跳转到内核

}