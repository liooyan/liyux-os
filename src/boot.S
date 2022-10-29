#define __ASM__ 1
#include <boot.h>

#define MB_FLAGS    MULTIBOOT_MEMORY_INFO | MULTIBOOT_PAGE_ALIGN

.section .multiboot
    .long MULTIBOOT_MAGIC
    .long MB_FLAGS
    .long CHECKSUM(MB_FLAGS)


.section .bss
    .align 16
    /* 零时栈 */
    stack_bottom:
        .skip STACK_SIZE, 0
    stack_top:

.section .text
    .global _start
    _start:
    /*关闭中断*/
        cli

        movl $stack_top, %esp
        pushl %ebx
        call _set_gdt # 进入main方法
        call _setup_init # 进入main方法

    1:
        hlt
            jmp 1b

##保护模式内存
        _set_gdt :
            call _init_gdt
            /* 更新段寄存器 */
            movw $BOOT_GDT_DATA, %cx
            movw %cx, %es
            movw %cx, %ds
            movw %cx, %fs
            movw %cx, %gs
            movw %cx, %ss

            /* 更新 CS:EIP */
            subl $8, %esp
            movw $BOOT_GDT_CODE,4(%esp)
            movl $_after_set_gdt,(%esp)
            retf
        _after_set_gdt:
            ret

