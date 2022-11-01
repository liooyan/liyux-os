.section .text
    .global _lcall
    _lcall:
        mov 4(%esp),%eax
        mov %esp,%ebx
        pushl %eax
        pushl $0
        lcall (%esp)
        movl %ebx,%esp
        ret
