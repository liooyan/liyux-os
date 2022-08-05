//
// Created by 李延 on 2022/7/25.
//

#include "kernel/tty.h"
#include "arch/x86/boot/multiboot.h"
#include "arch/x86/pic.h"
#include "arch/x86/cpu.h"
#include "arch/x86/idt.h"
#include "arch/x86/pci.h"
#include "arch/x86/ahci.h"
void _kernel_init() {
    _init_tty(multiboot_info->framebuffer_width, multiboot_info->framebuffer_height);
    _init_idt();
    _init_pic(80,88);
     _pci_probe();
    _ahci_init();
    sti();
}