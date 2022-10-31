#define __ASM__ 1
#include <init/boot.h>

#define MB_FLAGS    MULTIBOOT_MEMORY_INFO | MULTIBOOT_PAGE_ALIGN

.section .multiboot
    .long MULTIBOOT_MAGIC
    .long MB_FLAGS
    .long CHECKSUM(MB_FLAGS)


.section .bss
    .global stack_top
    .align 16
    /* 零时栈 */
    stack_bottom:
        .skip STACK_SIZE, 0
    stack_top:

.section .setup_heap
     heap_start:
        .skip HEAP_SIZE, 0
.section .text
    .global _start
    _start:
    /*关闭中断*/
        movl $stack_top, %esp
        pushl $heap_start
        pushl %ebx
        cli
        call _setup_init
    1:
        hlt
            jmp 1b
