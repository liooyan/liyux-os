#include "boot.h"


.section .text
    .global _start
    _start:
        movq %rax,%rdi
        movq %rbx,%rsi
        call main
    1:
        hlt
            jmp 1b

