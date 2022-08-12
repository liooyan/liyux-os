
//
// Created by 李延 on 2022/7/25.
//

#include "multiboot.h"
#include "tty.h"
#include "stdio.h"
#include "elf64.h"

int _setup_init(multiboot_info_t *multiboot_info) {
    _init_tty(multiboot_info->framebuffer_width, multiboot_info->framebuffer_height);
    kprintf("The display initialization is complete\n");


    //加载64位内核代码
    multiboot_uint32_t mb_flags = multiboot_info->flags;
    if (mb_flags & MULTIBOOT_INFO_MODS) {
        multiboot_uint32_t mods_count = multiboot_info->mods_count;
        multiboot_uint32_t mods_addr = multiboot_info->mods_addr;
        kprintf("find module num %d\n", mods_count);
        for (uint32_t mod = 0; mod < mods_count; mod++) {
            multiboot_module_t *module = (multiboot_module_t *) (mods_addr + (mod * sizeof(multiboot_module_t)));
            const char *module_string = (const char *) module->cmdline;
            kprintf("module [%s] start at [%d], end at [%d]\n", module_string, module->mod_start, module->mod_end);

            load_elf64(module->mod_start, module->mod_end);
        }

    }

    return 1;
}