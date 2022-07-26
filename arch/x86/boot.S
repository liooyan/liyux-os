#define __ASM__ 1
#include <arch/x86/boot/boot.h>

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
    /*关闭中断*/
        cli

        movl $stack_top, %esp
        movl %ebx,multiboot_info
        call _mem_init
    _after_mem_init:
         call _init_multiboot_info

        ._start_loop:
            jmp ._start_loop


    ##初始化内存
    _mem_init :

        call _init_gdt
        subl $6, %esp
        movl $_gdt,2(%esp)
        movw _gdt_limit,%ax
        movw %ax,(%esp)
        lgdt (%esp)
        addl $6, %esp
        /* 更新段寄存器 */
        movw $BOOT_GDT_DATA, %cx
        movw %cx, %es
        movw %cx, %ds
        movw %cx, %fs
        movw %cx, %gs
        movw %cx, %ss

        /* 更新 CS:EIP */
        subl $6, %esp
        movw $BOOT_GDT_CODE,4(%esp)
        movl $_after_mem_init,(%esp)
        retf