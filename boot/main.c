#include "shared/kmem.h"

//
// Created by Administrator on 2022/8/15.
//
int main(boot_params_t *boot_params){

    return  boot_params->kernel.start_addr;
}
