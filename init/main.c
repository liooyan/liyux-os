#include "shared/kmem.h"

//
// Created by Administrator on 2022/8/15.
//
int main(boot_params *hold_mem){

    return hold_mem->kernel[0].start_addr;
}
