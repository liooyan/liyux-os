//
// Created by Administrator on 2022/8/12.
//
#include "elf64.h"
#include "stdio.h"
#include "malloc.h"
#include "string.h"


#define BOOT_MODULE "boot"
#define KERNEL_MODULE "kernel"

static void load_elf64(uint32_t start_addr, uint32_t end_addr, Elf64_Meg *elf64_meg) {
    Elf64_Ehdr *elf_hdr = (Elf64_Ehdr *) start_addr;
    Elf64_Half elf_hdr_table_num = elf_hdr->e_shnum;


    Addr_section *table_addr = &elf64_meg->addr_section;
    elf64_meg->start_addr = elf_hdr->e_entry;
    elf64_meg->elf_hdr_table_num = elf_hdr_table_num;
    char *strtab_start;
    //find 字符串表
    for (int i = 0; i < elf_hdr_table_num; ++i) {
        Elf64_Shdr *shdr = (start_addr + elf_hdr->e_shoff + sizeof(Elf64_Shdr) * i);
        if (shdr->sh_type == SHT_STRTAB) {
            strtab_start = start_addr + shdr->sh_offset;
        }
    }


    //解析每个节
    for (int i = 0; i < elf_hdr_table_num; ++i) {
        Elf64_Shdr *shdr = (start_addr + elf_hdr->e_shoff + sizeof(Elf64_Shdr) * i);
        Addr_section *next_table = table_addr + i;
        char *name = strtab_start + shdr->sh_name;
        next_table->mapping_addr = shdr->sh_addr;
        next_table->start_addr = start_addr + shdr->sh_offset;
        next_table->addr_size = shdr->sh_size;
        strcopy(name, (char *) &next_table->name);
    }
}

void loading_kernel(multiboot_info_t *multiboot_info, boot_params_t *boot_params) {
    multiboot_uint32_t mb_flags = multiboot_info->flags;
    if (mb_flags & MULTIBOOT_INFO_MODS) {
        multiboot_uint32_t mods_count = multiboot_info->mods_count;
        multiboot_uint32_t mods_addr = multiboot_info->mods_addr;
        kprintf("find module num %d\n", mods_count);
        for (uint32_t mod = 0; mod < mods_count; mod++) {
            multiboot_module_t *module = (multiboot_module_t *) (mods_addr + (mod * sizeof(multiboot_module_t)));
            const char *module_string = (const char *) module->cmdline;
            kprintf("module [%s] loading ,start at [0x%x], end at [0x%x]\n", module_string, module->mod_start,
                    module->mod_end);
            if (streq(BOOT_MODULE, module_string)) {
                load_elf64(module->mod_start, module->mod_end, &boot_params->boot);
                kprintf("module [%s]  loaded, code start is [0x%llx], hdr table num is [%d]\n", module_string,
                        boot_params->kernel.start_addr, boot_params->kernel.elf_hdr_table_num);
            } else if (streq(KERNEL_MODULE, module_string)) {


                load_elf64(module->mod_start, module->mod_end, &boot_params->kernel);
                kprintf("module [%s]  loaded, code start is [0x%llx], hdr table num is [%d]\n", module_string,
                        boot_params->kernel.start_addr, boot_params->kernel.elf_hdr_table_num);
            }

        }
    }
}

