//
// Created by Administrator on 2022/8/12.
//
#include "elf64.h"
#include "stdio.h"
#include "malloc.h"


Elf64_Meg *load_elf64(uint32_t start_addr, uint32_t end_addr) {
    Elf64_Ehdr *elf_hdr = (Elf64_Ehdr *) start_addr;
    Elf64_Half elf_hdr_table_num = elf_hdr->e_shnum;


    Elf64_Meg *elf64_meg = malloc(sizeof(Elf64_Meg));
    Elf64_Table_Addr *table_addr = malloc(sizeof(Elf64_Table_Addr) * elf_hdr_table_num);
    elf64_meg->start_addr = elf_hdr->e_entry;
    elf64_meg->elf_hdr_table = table_addr;
    elf64_meg->elf_hdr_table_num = elf_hdr_table_num;

    //解析每个节
    for (int i = 0; i < elf_hdr_table_num; ++i) {
        Elf64_Shdr *shdr = (start_addr+elf_hdr->e_shoff+sizeof(Elf64_Shdr)*i);
        Elf64_Table_Addr *next_table = table_addr+i;

        next_table->start_addr = shdr->sh_addr;
        next_table->start_addr = start_addr+shdr->sh_offset;
        next_table->addr_size = shdr->sh_size;

    }


    return elf64_meg;
}
