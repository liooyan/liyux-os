//
// Created by Administrator on 2022/8/12.
//
#include "init/multiboot.h"
#include "lib/stdio.h"
#include "lib/int_ll32.h"
#include "lib/string.h"
#include "lib/elf32.h"
#include "mm/malloc.h"

static void load_elf32(uint32_t start_addr, uint32_t end_addr, elf32_meg_t *elf32_meg) {
    Elf32_Ehdr *elf_hdr = (Elf32_Ehdr *) start_addr;
    Elf32_Half elf_hdr_table_num = elf_hdr->e_shnum;


    elf32_meg->start_addr = elf_hdr->e_entry;
    elf32_meg->elf_hdr_table_num = elf_hdr_table_num;
    elf32_meg->addr_section = malloc(sizeof(addr_section_t) * elf_hdr_table_num);
    addr_section_t *table_addr = elf32_meg->addr_section;

    char *strtab_start;
    //find 字符串表
    for (int i = 0; i < elf_hdr_table_num; ++i) {
        Elf32_Shdr *shdr = (start_addr + elf_hdr->e_shoff + sizeof(Elf32_Shdr) * i);
        if (shdr->sh_type == SHT_STRTAB) {
            strtab_start = start_addr + shdr->sh_offset;
        }
    }


    //解析每个节
    for (int i = 0; i < elf_hdr_table_num; ++i) {
        Elf32_Shdr *shdr = (start_addr + elf_hdr->e_shoff + sizeof(Elf32_Shdr) * i);
        addr_section_t *next_table = table_addr + i;
        char *name = strtab_start + shdr->sh_name;
        next_table->mapping_addr = shdr->sh_addr;
        next_table->start_addr = start_addr + shdr->sh_offset;
        next_table->addr_size = shdr->sh_size;
        strcopy(name, (char *) &next_table->name);
    }
}

elf32_meg_t *loading_task(multiboot_info_t *multiboot_info,u32 *size) {
    elf32_meg_t *elf32_meg;
    multiboot_uint32_t mb_flags = multiboot_info->flags;
    if (mb_flags & MULTIBOOT_INFO_MODS) {
        multiboot_uint32_t mods_count = multiboot_info->mods_count;
        multiboot_uint32_t mods_addr = multiboot_info->mods_addr;
        *size = mods_count;

        elf32_meg =  malloc(sizeof(elf32_meg_t) * mods_count);


        kprintf("find module num %d\n", mods_count);
        for (uint32_t mod = 0; mod < mods_count; mod++) {
            elf32_meg_t *next_elf32_meg = elf32_meg + mod;
            multiboot_module_t *module = (multiboot_module_t *) (mods_addr + (mod * sizeof(multiboot_module_t)));
            const char *module_string = (const char *) module->cmdline;
            kprintf("module [%s] loading ,start at [0x%x], end at [0x%x]\n", module_string, module->mod_start,
                    module->mod_end);
            load_elf32(module->mod_start, module->mod_end, next_elf32_meg);

        }
    }
    return elf32_meg;
}

