//
// Created by root on 22-8-13.
//

/**
 * 记录kmem的内存分配情况
 */
#ifndef LIYUX_OS_KMEM_H
#define LIYUX_OS_KMEM_H

#define X86_CR4_PAE    0x00000020
#define MSR_EFER    0xc0000080 /* extended feature register */
#define _EFER_LME        8  /* Long mode enable */

#define X86_CR0_PE    0x00000001 /* Protection Enable */
#define X86_CR0_PG    0x80000000 /* Paging */

#define X86_MEM_PAGE_SIZE 4096

#define MEM_PAGE_ATTR(RW, MP_US, P) P | RW << 1 | MP_US << 2
#define MEM_GET_PAGE_ADDR(ADD) (ADD & 0xfffffffffffff000)

#define MEM_PAGE(ADD, RW, MP_US, P) MEM_GET_PAGE_ADDR(ADD) | MEM_PAGE_ATTR(RW, MP_US, P)




//读写权限
#define MP_RW_R 0
#define MP_RW_W 1

#define MP_US_R0 0
#define MP_US_RALL 1

#define MP_P_HAVE 1
#define MP_P_NONE 0


#include "stdint.h"

typedef struct {
    uint64_t mapping_addr; //映射起始地址
    uint32_t start_addr; //当前段的物理起始地址
    uint32_t addr_size; //当前段的物理结束地址
    char name[15];
} Addr_section;

typedef struct {
    uint64_t start_addr; //代码入库地址
    uint64_t elf_hdr_table_num;
    Addr_section *addr_section;

} Elf64_Meg;


typedef struct {
    Addr_section kernel[64]; //内核所用的空间
} boot_params;


typedef struct {
    uint16_t _gdt_limit;
    uint32_t *gdt;
} __attribute__((packed)) gdt_index_t;


void cut64(boot_params *hold_mem, Elf64_Meg *kernel_info);

#endif //LIYUX_OS_KMEM_H
