.section .text
    .global _task_lcall
    _task_lcall:
        mov 4(%esp),%eax
        mov %esp,%ebx
        pushl %eax
        pushl $0
        lcall (%esp)
        movl %ebx,%esp
        ret
