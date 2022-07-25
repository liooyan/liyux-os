#define __ASM__ 1
#include <arch/x86/boot/multiboot.h>

#define MB_FLAGS    MULTIBOOT_MEMORY_INFO | MULTIBOOT_PAGE_ALIGN
#define KPG_SIZE    10*4096

.section .multiboot
    .long MULTIBOOT_MAGIC
    .long MB_FLAGS
    .long CHECKSUM(MB_FLAGS)

.section .bss
    .global mb_info
    .align 16
    /* 为Multiboot info struct 预留空间 */
    mb_info:
        .skip 4096
    /* 根据System V ABI，栈地址必须16字节对齐 */
    /* 这里只是一个临时栈，在_hhk_init里面我们会初始化内核专用栈 */
    stack_bottom:
        .skip 16318 * 2, 0
    stack_top:




.section .text
    .global _start
    _start:
        movl $stack_top, %esp
        call _init_main
        ret