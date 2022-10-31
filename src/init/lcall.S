.section .text
    .global _lcall
    _lcall:
    /*关闭中断*/
        mov 4(%esp),%eax
        pushl %eax
        pushl $0
        lcall (%esp)

