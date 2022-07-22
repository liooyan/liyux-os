#ifndef MULTIBOOT_HEADER
#define MULTIBOOT_HEADER

// References https://www.gnu.org/software/grub/manual/multiboot/multiboot.html

#define MULTIBOOT_MAGIC 0x1BADB002
#define CHECKSUM(flags) -(MULTIBOOT_MAGIC + flags)

#endif