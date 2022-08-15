//
// Created by root on 22-8-12.
//

#include "kmem.h"
#include "malloc.h"
#include "x86/boot/cpu.h"
#include "tty.h"

static uint64_t *base_page;


/**
 * 设在临时的64位堆内存，设置2两个4G的内存
 * @param hold_mem
 */
static void set_gdt(kernel_hold_mem *hold_mem) {

    int gdt_len = 4;

    uint64_t *gdt = malloc(sizeof(uint64_t) * gdt_len);
    //设在3个段
    gdt[0] = 0x0000000000000000;
    gdt[1] = 0x00af9a000000ffff; //code
    gdt[2] = 0x00cf92000000ffff; //data
    idt_index_t idt_index;
    idt_index._gdt_limit = sizeof(uint64_t) * gdt_len;
    idt_index.gdt = gdt;
    cpu_lgdt((uint32_t *) &idt_index);

}

static void set_idt() {

    int gdt_len = 256;

    uint64_t *gdt = malloc(sizeof(uint64_t) * gdt_len);
    for (int i = 0; i < gdt_len; ++i) {
            gdt[i] = 0x860000100000;
    }
    idt_index_t idt_index;
    idt_index._gdt_limit = sizeof(uint64_t) * gdt_len;
    idt_index.gdt = gdt;
    cpu_lidt((uint32_t *) &idt_index);

}

static void set_page_2( Addr_section *addrSection) {
    uint32_t mapping_addr = addrSection->mapping_addr;
    uint32_t start_addr = addrSection->start_addr;
    uint32_t addr_size = addrSection->addr_size;
    //对齐start
    if(start_addr % X86_MEM_PAGE_SIZE != 0){
        uint32_t residue = start_addr % X86_MEM_PAGE_SIZE;
        start_addr -=residue;
        mapping_addr -=residue;
        addr_size+= residue;
    }

    int page_size = addr_size % X86_MEM_PAGE_SIZE == 0 ? addr_size / X86_MEM_PAGE_SIZE : addr_size / X86_MEM_PAGE_SIZE + 1;
    for (int i = 0; i < page_size; ++i) {
        uint32_t truly_addr = start_addr + i * X86_MEM_PAGE_SIZE;
        uint32_t mapping_addr_find = mapping_addr + i * X86_MEM_PAGE_SIZE;
        uint32_t *page_index_bash = (uint32_t *) base_page;
        for (int leve = 0; leve < 2; ++leve) {
            //获取当前的索引
            uint32_t index = (mapping_addr_find >> (22 - leve*10 )) & 0x3ff;
            uint32_t *page_index = page_index_bash + index;
            //还没有分配,分配新空间
            if (*page_index == 0) {
                //分配
                if (leve == 1) {
                    *page_index = MEM_PAGE((uint32_t) truly_addr, MP_RW_W, MP_US_R0, MP_P_HAVE);
                    break;
                } else {
                    uint32_t *new_p = malloc_4k(X86_MEM_PAGE_SIZE);
                    *page_index = MEM_PAGE((uint32_t) new_p, MP_RW_W, MP_US_R0, MP_P_HAVE);
                    page_index_bash = MEM_GET_PAGE_ADDR((uint32_t)new_p);
                }
            } else{
                page_index_bash =MEM_GET_PAGE_ADDR((uint32_t) *page_index_bash);
            }
        }

    }
}


static void set_page( Addr_section *addrSection) {
    uint64_t mapping_addr = addrSection->mapping_addr;
    uint32_t start_addr = addrSection->start_addr;
    uint32_t addr_size = addrSection->addr_size;
    //对齐start
    if(start_addr % X86_MEM_PAGE_SIZE != 0){
        uint32_t residue = start_addr % X86_MEM_PAGE_SIZE;
        start_addr -=residue;
        mapping_addr -=residue;
        addr_size+= residue;
    }

    int page_size = addr_size % X86_MEM_PAGE_SIZE == 0 ? addr_size / X86_MEM_PAGE_SIZE : addr_size / X86_MEM_PAGE_SIZE + 1;
    for (int i = 0; i < page_size; ++i) {
        uint64_t truly_addr = start_addr + i * X86_MEM_PAGE_SIZE;
        uint64_t mapping_addr_find = mapping_addr + i * X86_MEM_PAGE_SIZE;
        uint64_t *page_index_bash = (uint64_t *) base_page;
        for (int leve = 0; leve < 4; ++leve) {
            //获取当前的索引
            uint32_t index = (mapping_addr_find >> (39 - leve*9 )) & 0x3ff;
            uint64_t *page_index = page_index_bash + index;
            //还没有分配,分配新空间
            if (*page_index == 0) {
                //分配
                if (leve == 3) {
                    *page_index = MEM_PAGE((uint64_t) truly_addr, MP_RW_W, MP_US_R0, MP_P_HAVE);
                    break;
                } else {
                    uint64_t *new_p = malloc_4k(X86_MEM_PAGE_SIZE);
                    *page_index = MEM_PAGE((uint64_t) new_p, MP_RW_W, MP_US_R0, MP_P_HAVE);
                    page_index_bash = MEM_GET_PAGE_ADDR((uint64_t)new_p);
                }
            } else{
                page_index_bash =MEM_GET_PAGE_ADDR((uint64_t) *page_index_bash);
            }
        }

    }
}

/**
 * 设在内存分页，
 * @param hold_mem
 * @param kernel_info
 */
static void set_mem_page(kernel_hold_mem *hold_mem, Elf64_Meg *kernel_info) {
    base_page = malloc_4k(X86_MEM_PAGE_SIZE);
    Addr_section setupAddr = {
            .mapping_addr = 0x00100000,
            .start_addr = 0x00100000,
            .addr_size = 0x40000
    };
    set_page(&setupAddr);
    //分配kernel
    for (uint64_t i = 0; i < kernel_info->elf_hdr_table_num; ++i) {
        Addr_section *addrSection = kernel_info->addr_section + i;
        if(addrSection->addr_size >0 && addrSection->mapping_addr > 0){
            set_page(addrSection);
            Addr_section oldAddrSection = {
                    .mapping_addr = addrSection->start_addr,
                    .addr_size = addrSection->addr_size,
                    .start_addr = addrSection->start_addr
            };
            set_page(&oldAddrSection);
        }
    }

    //分配堆
    Addr_section addrSection = {
            .mapping_addr = malloc_addr,
            .addr_size = malloc_addr,
            .start_addr = malloc_heap_size
    };
    set_page(&addrSection);

    //分配vga显示空间
    Addr_section vgaAddrSection  = {
            .mapping_addr = VGA_BASH_ADDR,
            .addr_size = VGA_BASH_ADDR,
            .start_addr = X86_MEM_PAGE_SIZE
    };
    set_page(&vgaAddrSection);

    // 将分页地址写入到cr3中
    cpu_lcr3((uint32_t)base_page);
}


void cut64(kernel_hold_mem *hold_mem, Elf64_Meg *kernel_info) {

    //设置一个空的idt
    //set_idt();

    uint32_t  cr4 = cpu_rcr4();
    cpu_lcr4(X86_CR4_PAE| cr4);

    set_gdt(hold_mem); //设置64位的段
    set_mem_page(hold_mem, kernel_info);//设置4段的分页
    rdmsr();
    unsigned int cr0 = cpu_rcr0();
    cpu_lcr0(0x80010000 | cr0);


}



