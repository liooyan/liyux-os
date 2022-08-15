#include <boot.h>



.section .text
    .global _jump_to_64
    _jump_to_64:
        movl -4(%esp),%eax
        /* 更新 CS:EIP */
        ljmp $BOOT_GDT_CODE,$0xc8000000



