#include <stddef.h>
#include "shared/kmem.h"
#include "liyux/init.h"
#include "mm/early_res.h"
#include "processor.h"

#define MAX_EARLY_RES_X 500


void strcopy(char *source, char *target);
int streq(const char *source,const char *target);

static struct early_res early_res_x[MAX_EARLY_RES_X] __initdata;

struct early_res *early_res  = &early_res_x[0];

struct early_res *find_free_early_res() {
    for (int i = 0; i < MAX_EARLY_RES_X; ++i) {
        struct early_res *find = early_res+i;
        if(find->start == 0){
            return find;
        }
    }
    return NULL;
}


void init_data_registere(Elf64_Meg boot) {

    for (int i = 0; i < 50; i++) {
        Addr_section *addrSection = &boot.addr_section[i];
        if (streq(".init.data",addrSection->name)) {
            struct early_res *free_early_res =    find_free_early_res();
            free_early_res->start =addrSection->start_addr;
            free_early_res->end = addrSection->start_addr+addrSection->addr_size;
            free_early_res->overlap_ok  = 0;
            strcopy(".init.data",free_early_res->name);
            break;
        }
    }
}
void strcopy(char *source, char *target) {
    while (*source) {
        *target = *source;
        source++;
        target++;
    }
    *target = 0;

}
int streq(const char *source,const char *target){
    while (*source &&  *target) {
        if( *target != *source){
            return 0;
        }
        source++;
        target++;
    }
    if(!*source &&  !*target){
        return 1;
    } else{
        return 0;
    }

}


//
// Created by Administrator on 2022/8/15.
//
int main(boot_params_t *boot_params) {

    init_data_registere(boot_params->boot);


    unsigned char name[17];
    unsigned int eax;
    cpuid(0x0, &eax, (unsigned int *) &name, (unsigned int *) &name[4], (unsigned int *) &name[8]);
    return 1;
}
