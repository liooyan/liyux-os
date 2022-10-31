#ifndef BOOT_HEADER
#define BOOT_HEADER


#define MULTIBOOT_MAGIC 0x1BADB002
#define CHECKSUM(flags) -(MULTIBOOT_MAGIC + flags)
/* Align all boot modules on i386 page (4KB) boundaries. */
#define MULTIBOOT_PAGE_ALIGN                    0x00000001

/* Must pass memory information to OS. */
#define MULTIBOOT_MEMORY_INFO                   0x00000002


#define  STACK_SIZE 512
#define  HEAP_SIZE 4096*200

#define BOOT_TSS_SELECTOR 0x18



#define BOOT_R3_GDT_CODE 0x1B
#define BOOT_R3_GDT_DATA 0x13

#endif