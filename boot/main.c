#include <stddef.h>
#include "shared/kmem.h"
#include "liyux/init.h"
#include "mm/early_res.h"
#include "processor.h"
#include "liyux/cpu.h"

#define MAX_EARLY_RES_X 500
#define  PHYSICAL_MEMORY  0xffff888000000000

void strcopy(char *source, char *target);
int streq(const char *source, const char *target);

void _jump_kernel();

uint64_t boot_mm_start_addr;


static struct early_res early_res_x[MAX_EARLY_RES_X] __initdata;
boot_params_t boot_params_c __initdata;
boot_params_t *boot_params_c_x = &boot_params_c;

struct early_res *early_res = &early_res_x[0];

void init_boot_mm(boot_params_t *boot_params) {
    Elf64_Meg *boot = &boot_params->boot;
    uint64_t max_end_addr = 0;
    for (int i = 0; i < 50; i++) {
        Addr_section *addrSection = &boot->addr_section[i];
        uint64_t end_addr = addrSection->start_addr + addrSection->addr_size;
        if (max_end_addr < end_addr) {
            max_end_addr = end_addr;
        }
    }
    uint64_t page_size = 4096;
    if(max_end_addr%page_size != 0){
        uint32_t residue = page_size- max_end_addr%page_size;
        max_end_addr+= residue;
    }
    boot_mm_start_addr = max_end_addr;
}


struct early_res *find_free_early_res() {
    for (int i = 0; i < MAX_EARLY_RES_X; ++i) {
        struct early_res *find = early_res + i;
        if (find->start == 0) {
            return find;
        }
    }
    return NULL;
}


void init_data_registere(Elf64_Meg *boot) {

    for (int i = 0; i < 50; i++) {
        Addr_section *addrSection = &boot->addr_section[i];
        if (streq(".init.data", addrSection->name)) {
            struct early_res *free_early_res = find_free_early_res();
            free_early_res->start = addrSection->start_addr;
            free_early_res->end = addrSection->start_addr + addrSection->addr_size;
            free_early_res->overlap_ok = 0;
            strcopy(".init.data", free_early_res->name);
            break;
        }
    }
}

void kernel_mm_registere(Elf64_Meg *boot) {

    uint64_t max_end_addr = 0;
    uint64_t min_start_addr = 0xffffffffffff;
    for (int i = 0; i < 50; i++) {
        Addr_section *addrSection = &boot->addr_section[i];
        if (addrSection->start_addr == 0) {
            continue;
        }
        uint64_t end_addr = addrSection->start_addr + addrSection->addr_size;
        if (max_end_addr < end_addr) {
            max_end_addr = end_addr;
        }
        if (min_start_addr > addrSection->start_addr) {
            min_start_addr = end_addr;
        }
    }
    struct early_res *free_early_res = find_free_early_res();
    free_early_res->start = min_start_addr;
    free_early_res->end = max_end_addr;
    free_early_res->overlap_ok = 1;
    strcopy(".kernel", free_early_res->name);
}

void gdt_registere(Addr_section *addrSection) {

    struct early_res *free_early_res = find_free_early_res();
    free_early_res->start = addrSection->start_addr;
    free_early_res->end = addrSection->start_addr + addrSection->addr_size;
    free_early_res->overlap_ok = 1;
    strcopy(".gdt", free_early_res->name);
}

void pge_base_registere() {

    struct early_res *free_early_res = find_free_early_res();
    free_early_res->start = cpu_rcr3();
    free_early_res->end = 0x1000;
    free_early_res->overlap_ok = 1;
    strcopy(".gdt", free_early_res->name);
}

void strcopy(char *source, char *target) {
    while (*source) {
        *target = *source;
        source++;
        target++;
    }
    *target = 0;

}

int streq(const char *source, const char *target) {
    while (*source && *target) {
        if (*target != *source) {
            return 0;
        }
        source++;
        target++;
    }
    if (!*source && !*target) {
        return 1;
    } else {
        return 0;
    }

}



int boot_malloc_is_use(uint64_t start, uint64_t end) {
    for (int i = 0; i < MAX_EARLY_RES_X; ++i) {
        struct early_res *find = early_res + i;
        if (!(find->start > end || find->end < start)) {
            return 1;
        }
    }
    return 0;
}
uint64_t boot_malloc_find_in_physical(uint64_t start, uint64_t size) {
    kernel_hold_mem_t *kernelHoldMem = &boot_params_c_x->kernel_hold_mem;
    for (int i = 0; i < kernelHoldMem->mmap_size; ++i) {
        memory_map_t *memory_map= &kernelHoldMem->mmap[i];
        if(memory_map->type != 1){
            continue;
        }
        uint64_t memory_start = (memory_map->base_addr_high << 32)+ memory_map->base_addr_low;
        uint64_t memory_length = (memory_map->length_high << 32)+ memory_map->length_low;
        uint64_t memory_end =memory_start+memory_length;
        if(memory_end >= start && memory_end- start >= size){
            return start > memory_start? start:memory_start;
        }
    }
    return -1;
}

void *boot_malloc(uint64_t  size){
    uint64_t  page_size = 4096;
    if(size%page_size != 0){
        uint32_t residue = page_size- size%page_size;
        size+= residue;
    }
    uint64_t addr = boot_mm_start_addr;
    while (1){
         addr = boot_malloc_find_in_physical(addr,size);
         if(boot_malloc_is_use(addr,addr+size-1)){
             addr +=size;
         } else{
             break;
         }
    }
    struct early_res *free_early_res = find_free_early_res();
    free_early_res->start =addr;
    free_early_res->end = addr+size-1;
    free_early_res->overlap_ok = 1;
    strcopy(".boot.mm", free_early_res->name);
    for (int i = 0; i < size; ++i) {
        ((char *)addr)[i] = 0;
    }
    return (void *)addr;

}
void jmp_kernel(){
    uint64_t  addr = 0xffffffff80000000;
    uint64_t *ad = &addr;
    asm volatile("call *%0" : :
            "m" (addr));
}

static void set_page(Addr_section_64 *addrSection) {
    uint64_t mapping_addr = addrSection->mapping_addr;
    uint64_t start_addr = addrSection->start_addr;
    uint32_t addr_size = addrSection->addr_size;
    //对齐start
    if (start_addr % X86_MEM_PAGE_SIZE != 0) {
        uint32_t residue = start_addr % X86_MEM_PAGE_SIZE;
        start_addr -= residue;
        mapping_addr -= residue;
        addr_size += residue;
    }

    int page_size =
            addr_size % X86_MEM_PAGE_SIZE == 0 ? addr_size / X86_MEM_PAGE_SIZE : addr_size / X86_MEM_PAGE_SIZE + 1;
    for (int i = 0; i < page_size; ++i) {
        uint64_t truly_addr = start_addr + i * X86_MEM_PAGE_SIZE;
        uint64_t mapping_addr_find = mapping_addr + i * X86_MEM_PAGE_SIZE;
        uint64_t *page_index_bash = (uint64_t *) cpu_rcr3();
        for (int leve = 0; leve < 4; ++leve) {
            //获取当前的索引
            uint32_t index = (mapping_addr_find >> (39 - leve * 9)) & 0x1ff;
            uint64_t *page_index = page_index_bash + index;
            //还没有分配,分配新空间
            if (*page_index == 0) {
                //分配
                if (leve == 3) {
                    *page_index = MEM_PAGE((uint64_t) truly_addr, MP_RW_W, MP_US_R0, MP_P_HAVE);
                    break;
                } else {
                    uint64_t *new_p = boot_malloc(X86_MEM_PAGE_SIZE);
                    *page_index = MEM_PAGE((uint64_t) new_p, MP_RW_W, MP_US_R0, MP_P_HAVE);
                    page_index_bash = MEM_GET_PAGE_ADDR((uint64_t) new_p);
                }
            } else {
                if (leve == 3) {
                    break;
                }
                page_index_bash = MEM_GET_PAGE_ADDR((uint64_t) *page_index);
            }
        }

    }
}



void page_physical(){
    kernel_hold_mem_t *kernelHoldMem = &boot_params_c_x->kernel_hold_mem;
    for (int i = 0; i < kernelHoldMem->mmap_size; ++i) {
        memory_map_t *memory_map= &kernelHoldMem->mmap[i];
        if(memory_map->type != 1){
            continue;
        }
        uint64_t memory_start = (memory_map->base_addr_high << 32)+ memory_map->base_addr_low;
        uint64_t memory_length = (memory_map->length_high << 32)+ memory_map->length_low;
        Addr_section_64 addrSection64 = {
                .addr_size = memory_length,
                .start_addr = memory_start,
                .mapping_addr = PHYSICAL_MEMORY +memory_start
        };
        set_page(&addrSection64);
    }
}

void page_kernel(){
    Elf64_Meg *kernel_info = &boot_params_c_x->kernel;
    for (uint64_t i = 0; i < kernel_info->elf_hdr_table_num; ++i) {
        Addr_section *addrSection = kernel_info->addr_section + i;
        if (addrSection->addr_size > 0 && addrSection->mapping_addr > 0) {
            Addr_section_64 addrSection64 = {
                    .addr_size = addrSection->addr_size,
                    .start_addr = addrSection->start_addr,
                    .mapping_addr = addrSection->mapping_addr
            };
            set_page(&addrSection64);

        }
    }
}
//
// Created by Administrator on 2022/8/15.
//
int main(boot_params_t *boot_params) {
    boot_params_c = *boot_params;
    init_data_registere(&boot_params_c.boot);
    kernel_mm_registere(&boot_params_c.kernel);
    gdt_registere(&boot_params_c.gdt_addr);
    pge_base_registere();
    init_boot_mm(&boot_params_c);
    page_physical();
    page_kernel();
    jmp_kernel();
    return 0;
}