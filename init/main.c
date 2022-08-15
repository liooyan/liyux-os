#include "shared/kmem.h"

//
// Created by Administrator on 2022/8/15.
//
int main(kernel_hold_mem *hold_mem){

    return hold_mem->stack.start_addr;
}
