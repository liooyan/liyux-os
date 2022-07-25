#ifndef MULTIBOOT_HEADER
#define MULTIBOOT_HEADER

// References https://www.gnu.org/software/grub/manual/multiboot/multiboot.html

#define MULTIBOOT_MAGIC 0x1BADB002
#define CHECKSUM(flags) -(MULTIBOOT_MAGIC + flags)
/* Align all boot modules on i386 page (4KB) boundaries. */
#define MULTIBOOT_PAGE_ALIGN                    0x00000001

/* Must pass memory information to OS. */
#define MULTIBOOT_MEMORY_INFO                   0x00000002


#endif