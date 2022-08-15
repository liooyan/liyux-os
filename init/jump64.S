


.section .text
    .global main
    main:
        /* 更新 CS:EIP */
        addl $16, %esp
        addl $16, %esp
        retf

    1:
        hlt
            jmp 1b

