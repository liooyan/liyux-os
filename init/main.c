#include "arch/x86/boot/multiboot.h"

//
// Created by 李延 on 2022/7/25.
//

#include <arch/x86/boot/multiboot.h>
extern multiboot_info_t* multiboot_info;

int _kernel_init() {

    return 1;
}