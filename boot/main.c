#include <stddef.h>
#include "shared/kmem.h"
#include "liyux/init.h"
#include "mm/early_res.h"
#include "processor.h"
#include "liyux/cpu.h"

#define MAX_EARLY_RES_X 500


void strcopy(char *source, char *target);

int streq(const char *source, const char *target);

uint64_t boot_mm_start_addr;


static struct early_res early_res_x[MAX_EARLY_RES_X] __initdata;
  boot_params_t boot_params_c __initdata;


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


    unsigned char name[17];
    unsigned int eax;
    cpuid(0x0, &eax, (unsigned int *) &name, (unsigned int *) &name[4], (unsigned int *) &name[8]);
    return 1;
}
