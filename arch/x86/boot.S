#define __ASM__ 1
#include <arch/x86/boot/multiboot.h>

#define MB_FLAGS    MULTIBOOT_MEMORY_INFO | MULTIBOOT_PAGE_ALIGN
#define KPG_SIZE    10*4096

.section .multiboot
    .long MULTIBOOT_MAGIC
    .long MB_FLAGS
    .long CHECKSUM(MB_FLAGS)





.section .hhk_init
    .global _start
    _start:

        ret