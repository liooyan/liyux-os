//
// Created by 李延 on 2022/7/25.
//

#include "kernel/tty/tty.h"
#include "arch/x86/boot/multiboot.h"
#include "arch/x86/pic.h"
#include "arch/x86/cpu.h"
#include "arch/x86/idt.h"
#include "arch/x86/pci.h"
#include "arch/x86/ahci.h"
#include "arch/x86/gdt.h"

extern  gdt_descriptor_t *_gdt;
void _kernel_init() {
    _init_tty(multiboot_info->framebuffer_width, multiboot_info->framebuffer_height);
    for (int i = 0; i < 6; ++i) {
        gdt_print(i);
    }
    _init_idt();
    _init_pic(80, 88);
    _pci_probe();
    _ahci_init();
    sti();
}