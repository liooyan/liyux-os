//
// Created by Administrator on 2022/8/12.
//
#include "elf64.h"
#include "stdio.h"
#include "malloc.h"
#include "string.h"

static Elf64_Meg *load_elf64(uint32_t start_addr, uint32_t end_addr) {
    Elf64_Ehdr *elf_hdr = (Elf64_Ehdr *) start_addr;
    Elf64_Half elf_hdr_table_num = elf_hdr->e_shnum;


    Elf64_Meg *elf64_meg = malloc(sizeof(Elf64_Meg));
    Addr_section *table_addr = malloc(sizeof(Addr_section) * elf_hdr_table_num);
    elf64_meg->start_addr = elf_hdr->e_entry;
    elf64_meg->addr_section = table_addr;
    elf64_meg->elf_hdr_table_num = elf_hdr_table_num;
    char *strtab_start ;
    //find 字符串表
    for (int i = 0; i < elf_hdr_table_num; ++i) {
        Elf64_Shdr *shdr = (start_addr+elf_hdr->e_shoff+sizeof(Elf64_Shdr)*i);
        if (shdr->sh_type == SHT_STRTAB){
            strtab_start = start_addr+shdr->sh_offset;
        }
    }


    //解析每个节
    for (int i = 0; i < elf_hdr_table_num; ++i) {
        Elf64_Shdr *shdr = (start_addr+elf_hdr->e_shoff+sizeof(Elf64_Shdr)*i);
        Addr_section *next_table = table_addr+i;
        char *name =  strtab_start+shdr->sh_name;
        next_table->mapping_addr = shdr->sh_addr;
        next_table->start_addr = start_addr+shdr->sh_offset;
        next_table->addr_size = shdr->sh_size;
        strcopy(name, (char *) &next_table->name);
    }
    return elf64_meg;
}

Elf64_Meg *loading_kernel(multiboot_info_t *multiboot_info){
    multiboot_uint32_t mb_flags = multiboot_info->flags;
    if (mb_flags & MULTIBOOT_INFO_MODS) {
        multiboot_uint32_t mods_count = multiboot_info->mods_count;
        multiboot_uint32_t mods_addr = multiboot_info->mods_addr;
        kprintf("find module num %d\n", mods_count);
        for (uint32_t mod = 0; mod < mods_count; mod++) {
            multiboot_module_t *module = (multiboot_module_t *) (mods_addr + (mod * sizeof(multiboot_module_t)));
            const char *module_string = (const char *) module->cmdline;
            kprintf("module [%s] loading ,start at [0x%x], end at [0x%x]\n", module_string, module->mod_start, module->mod_end);
            Elf64_Meg *elf_info = load_elf64(module->mod_start, module->mod_end);
            kprintf("module [%s]  loaded, code start is [0x%llx], hdr table num is [%d]\n",module_string, elf_info->start_addr,elf_info->elf_hdr_table_num);
            return elf_info;
        }
    }
}

