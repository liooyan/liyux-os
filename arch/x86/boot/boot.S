#define __ASM__ 1
#include <arch/x86/boot/boot.h>

#define MB_FLAGS    MULTIBOOT_MEMORY_INFO | MULTIBOOT_PAGE_ALIGN


.section .multiboot
    .long MULTIBOOT_MAGIC
    .long MB_FLAGS
    .long CHECKSUM(MB_FLAGS)


.section .bss
    .align 16
    /* 零时栈 */
    stack_bottom:
        .skip 16318 * 2, 0
    stack_top:




.section .text
    .global _start
    _start:
    /*关闭中断*/
        cli

        movl $stack_top, %esp
        pushl %ebx
        call _setup_init # 进入main方法

    1:
        hlt
            jmp 1b

