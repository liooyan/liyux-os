#include "boot.h"


.section .text
    .global _start
    _start:
        /* 更新 CS:EIP */
        movw $BOOT_GDT_DATA, %cx
        movw %cx, %es
        movw %cx, %ds
        movw %cx, %fs
        movw %cx, %gs
        movw %cx, %ss
        movq $0xffffffffffffffff, %r8
        call main
    1:
        hlt
            jmp 1b

