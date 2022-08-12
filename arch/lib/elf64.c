//
// Created by Administrator on 2022/8/12.
//
#include "elf64.h"
#include "stdio.h"



void  load_elf64(uint32_t start_addr,uint32_t end_addr){
    Elf64_Ehdr *elf_hdr = (Elf64_Ehdr *) start_addr;

    Elf64_Addr  elf_entry = elf_hdr ->e_entry;
    Elf64_Half elf_hdr_table_num = elf_hdr->e_phnum;

    kprintf("elf code start addr is [0x%llx]\n", elf_entry);
    kprintf("elf hdr table num is [%d]\n", elf_hdr_table_num);


}
