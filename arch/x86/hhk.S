
#include <arch/x86/boot/boot.h>

.section .text
    .global _hhk_init
    _hhk_init:
        call _set_gdt
        call _set_mem_page
        ret





    ##保护模式内存
    _set_gdt :

        call _init_gdt
        subl $16, %esp
        movl $_gdt,2(%esp)
        movw _gdt_limit,%ax
        movw %ax,(%esp)
        lgdt (%esp)
        addl $16, %esp
        /* 更新段寄存器 */
        movw $BOOT_GDT_DATA, %cx
        movw %cx, %es
        movw %cx, %ds
        movw %cx, %fs
        movw %cx, %gs
        movw %cx, %ss

        /* 更新 CS:EIP */
        subl $BOOT_GDT_CODE, %esp
        movw $BOOT_GDT_CODE,4(%esp)
        movl $_after_set_gdt,(%esp)
        retf
    _after_set_gdt:
        ret


    _set_mem_page:

        pushl $_k_ptd

        call _inti_page
        addl $4, %esp
        ret