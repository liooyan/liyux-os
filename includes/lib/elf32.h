//
// Created by Administrator on 2022/8/12.
//

#ifndef LIYUX_OS_X86_64_ELF64_H
#define LIYUX_OS_X86_64_ELF64_H

#include "init/multiboot.h"
#include "lib/int_ll32.h"
typedef u16 Elf32_Half;
typedef u32 Elf32_Word;
typedef u32 Elf32_Off;
typedef u32 Elf32_Addr;

#define EI_NIDENT (16)

#define  SHT_STRTAB 3


typedef struct
{
    unsigned char	e_ident[EI_NIDENT];	/* Magic number and other info */
    Elf32_Half	e_type;			/* Object file type */
    Elf32_Half	e_machine;		/* Architecture */
    Elf32_Word	e_version;		/* Object file version */
    Elf32_Addr	e_entry;		/* Entry point virtual address */
    Elf32_Off	e_phoff;		/* Program header table file offset */
    Elf32_Off	e_shoff;		/* Section header table file offset */
    Elf32_Word	e_flags;		/* Processor-specific flags */
    Elf32_Half	e_ehsize;		/* ELF header size in bytes */
    Elf32_Half	e_phentsize;		/* Program header table entry size */
    Elf32_Half	e_phnum;		/* Program header table entry count */
    Elf32_Half	e_shentsize;		/* Section header table entry size */
    Elf32_Half	e_shnum;		/* Section header table entry count */
    Elf32_Half	e_shstrndx;		/* Section header string table index */
} Elf32_Ehdr;

typedef struct
{
    Elf32_Word	sh_name;		/* Section name (string tbl index) */
    Elf32_Word	sh_type;		/* Section type */
    Elf32_Word	sh_flags;		/* Section flags */
    Elf32_Addr	sh_addr;		/* Section virtual addr at execution */
    Elf32_Off	sh_offset;		/* Section file offset */
    Elf32_Word	sh_size;		/* Section size in bytes */
    Elf32_Word	sh_link;		/* Link to another section */
    Elf32_Word	sh_info;		/* Additional section information */
    Elf32_Word	sh_addralign;		/* Section alignment */
    Elf32_Word	sh_entsize;		/* Entry size if section holds table */
} Elf32_Shdr;


typedef struct {
    u32 mapping_addr; //映射起始地址
    u32 start_addr; //当前段的物理起始地址
    u32 addr_size; //当前段的物理结束地址
    char name[15];
} addr_section_t;


typedef struct {
    u32 start_addr; //代码入库地址
    u32 elf_hdr_table_num;
    addr_section_t *addr_section;

} elf32_meg_t;

elf32_meg_t *loading_task(multiboot_info_t *multiboot_info,u32 *size);

#endif //LIYUX_OS_X86_64_ELF64_H
