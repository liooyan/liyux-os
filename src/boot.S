#define __ASM__ 1
#include <boot.h>

#define MB_FLAGS    MULTIBOOT_MEMORY_INFO | MULTIBOOT_PAGE_ALIGN

.section .multiboot
    .long MULTIBOOT_MAGIC
    .long MB_FLAGS
    .long CHECKSUM(MB_FLAGS)


.section .bss
    .global stack_top
    .align 16
    /* 零时栈 */
    stack_bottom:
        .skip STACK_SIZE, 0
    stack_top:

.section .text
    .global _start,_work_to,_work_to2
    _start:
    /*关闭中断*/
        cli
        call _init_gdt
        ljmp $BOOT_TSS_SELECTOR,$0

    1:
        hlt
            jmp 1b
