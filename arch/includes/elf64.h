//
// Created by Administrator on 2022/8/12.
//

#ifndef LIYUX_OS_X86_64_ELF64_H
#define LIYUX_OS_X86_64_ELF64_H
#include "stdint.h"

typedef uint16_t Elf64_Half;
typedef uint32_t Elf64_Word;
typedef	int32_t  Elf64_Sword;
typedef uint64_t Elf64_Off;
typedef uint64_t Elf64_Addr;

#define EI_NIDENT (16)

typedef struct
{
    unsigned char	e_ident[EI_NIDENT];	/* Magic number and other info */
    Elf64_Half	e_type;			/* Object file type */
    Elf64_Half	e_machine;		/* Architecture */
    Elf64_Word	e_version;		/* Object file version */
    Elf64_Addr	e_entry;		/* Entry point virtual address */
    Elf64_Off	e_phoff;		/* Program header table file offset */
    Elf64_Off	e_shoff;		/* Section header table file offset */
    Elf64_Word	e_flags;		/* Processor-specific flags */
    Elf64_Half	e_ehsize;		/* ELF header size in bytes */
    Elf64_Half	e_phentsize;		/* Program header table entry size */
    Elf64_Half	e_phnum;		/* Program header table entry count */
    Elf64_Half	e_shentsize;		/* Section header table entry size */
    Elf64_Half	e_shnum;		/* Section header table entry count */
    Elf64_Half	e_shstrndx;		/* Section header string table index */
} Elf64_Ehdr;



void  load_elf64(uint32_t start_addr,uint32_t end_addr);
#endif //LIYUX_OS_X86_64_ELF64_H
