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
    .global _start,_work_to,_work_to2
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
            ljmp $BOOT_GDT_CODE,$_after_set_gdt
        _after_set_gdt:
            lcall $0x002b ,$0
            ret
        _work_to:
            call _setup_init # 进入main方法
            lcall $0x0030 ,$0
            iret
        _work_to2:
            call _setup_init # 进入main方法
            iret