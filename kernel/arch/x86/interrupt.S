#include <kernel/interrupts.h>



.macro isr_template vector, no_error_code=1
    .global _asm_isr\vector
    .type _asm_isr\vector, @function
    _asm_isr\vector:
        .if \no_error_code
            subl $4, %esp
            movl $0x0,(%esp)
        .endif
        subl $4, %esp
        movl $\vector,(%esp)
        jmp interrupt_wrapper
.endm

.section .text
    isr_template FAULT_DIVISION_ERROR


    interrupt_wrapper:
    pushl %eax
    movl 4(%esp),%eax
    pushl %ebx
    movl $_idt_function,%ebx
    call *(%ebx,%eax,4)
    iret
