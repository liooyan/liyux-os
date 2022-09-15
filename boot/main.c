#include "shared/kmem.h"
#include "shared/processor.h"

//
// Created by Administrator on 2022/8/15.
//
int main(boot_params_t *boot_params) {
    unsigned char name[17];
    unsigned int eax;
    cpuid(0x0,&eax,&name,&name[4],&name[8]);
    return (int) (&name + eax);
}
