#include <stddef.h>
#include "shared/kmem.h"
#include "processor.h"
#include "liyux/init.h"
#include "string.h"
#include "mm/early_res.h"

#define MAX_EARLY_RES_X 500
static struct early_res early_res_x[2] __initdata;


struct early_res *find_free_early_res() {
    for (int i = 0; i < MAX_EARLY_RES_X; ++i) {
        struct  early_res *find = early_res_x;
        if(find->start == 0){
            return find;
        }
    }
    return NULL;
}


void init_data_registere(Elf64_Meg boot) {

    for (int i = 0; i < sizeof(boot.addr_section) / sizeof(boot.addr_section[0]); ++i) {
        Addr_section addrSection = boot.addr_section[i];
        int name_eq = streq(".init.data", addrSection.name);
        if (name_eq) {
            struct early_res *free_early_res =    find_free_early_res();
            free_early_res->start =addrSection.start_addr;
            free_early_res->end = addrSection.start_addr+addrSection.addr_size;
            free_early_res->overlap_ok  = 0;
            strcopy(addrSection.name, (char *) &free_early_res->name);
            break;
        }
    }
}


//
// Created by Administrator on 2022/8/15.
//
int main(boot_params_t *boot_params) {

//    init_data_registere(boot_params->boot);
//
//
//    unsigned char name[17];
//    unsigned int eax;
//    cpuid(0x0, &eax, &name, &name[4], &name[8]);
    return 1;
}
