#define __ASM__ 1
#include <arch/x86/boot/boot.h>

#define MB_FLAGS    MULTIBOOT_MEMORY_INFO | MULTIBOOT_PAGE_ALIGN

#define KPG_SIZE    10*4096

.section .multiboot
    .long MULTIBOOT_MAGIC
    .long MB_FLAGS
    .long CHECKSUM(MB_FLAGS)

.section .kpg
    .global _k_ptd
    _k_ptd:
        .skip KPG_SIZE, 0

.section .bss
    .global mb_info
    .align 16
    /* 根据System V ABI，栈地址必须16字节对齐 */
    /* 这里只是一个临时栈，在_hhk_init里面我们会初始化内核专用栈 */
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
        call _kernel_init # 初始化内核

    1:
        hlt
            jmp 1b

