//
// Created by Administrator on 2022/8/12.
//

#ifndef LIYUX_OS_X86_64_ELF64_H
#define LIYUX_OS_X86_64_ELF64_H
#include "stdint.h"
#include "multiboot.h"
#include "kmem.h"
typedef uint16_t Elf64_Half;
typedef uint32_t Elf64_Word;
typedef	int32_t  Elf64_Sword;
typedef uint64_t Elf64_Off;
typedef uint64_t Elf64_Addr;

#define EI_NIDENT (16)

#define  SHT_STRTAB 3
typedef struct
{
    unsigned char	e_ident[EI_NIDENT];	/* 魔数固定的ELF， EI_NIDENT 为16个字节 */
    Elf64_Half	e_type;			/* 该文件的类型 2字节 */
    Elf64_Half	e_machine;		/* 该程序需要的体系架构 2字节 */
    Elf64_Word	e_version;		/* 文件的版本 4字节 */
    Elf64_Addr	e_entry;		/* 程序的入口地址 8字节 */
    Elf64_Off	e_phoff;		/* Program header table（程序） 在文件中的偏移量 8字节 */
    Elf64_Off	e_shoff;		/* Section header table（节头表） 在文件中的偏移量 8字节 */
    Elf64_Word	e_flags;		/* Processor-specific flags */
    Elf64_Half	e_ehsize;		/* 表示ELF header大小 2字节 */
    Elf64_Half	e_phentsize;		/* 表示Program header table中每一个条目的大小 2字节 */
    Elf64_Half	e_phnum;		/* 表示Program header table中有多少个条目 2字节 */
    Elf64_Half	e_shentsize;		/* 表示Section header table中的每一个条目的大小 2字节 */
    Elf64_Half	e_shnum;		/* 表示Section header table中有多少个条目 2字节 */
    Elf64_Half	e_shstrndx;		/* 包含节名称的字符串是第几个节 2字节 */
} Elf64_Ehdr;

typedef struct {
    uint32_t   sh_name;  // 节区名称相对于字符串表的位置偏移
    uint32_t   sh_type;  // 节区类型
    uint64_t   sh_flags;  // 节区标志位集合
    Elf64_Addr sh_addr;  // 节区装入内存的地址
    Elf64_Off  sh_offset;  // 节区相对于文件的位置偏移
    uint64_t   sh_size;  // 节区内容大小
    uint32_t   sh_link;  // 指定链接的节索引，与具体的节有关
    uint32_t   sh_info;  // 指定附加信息
    uint64_t   sh_addralign;  // 节装入内存的地址对齐要求
    uint64_t   sh_entsize;  // 指定某些节的固定表大小，与具体的节有关
} Elf64_Shdr;



#define KERNEL_ELF_NAME kernel




Elf64_Meg *loading_kernel(multiboot_info_t *multiboot_info);
#endif //LIYUX_OS_X86_64_ELF64_H
