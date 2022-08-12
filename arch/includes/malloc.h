//
// Created by Administrator on 2022/8/12.
//

#ifndef LIYUX_OS_X86_64_MALLOC_H
#define LIYUX_OS_X86_64_MALLOC_H
#include "stdint.h"

void *malloc(uint32_t size);

void heap_init(uint32_t addr,uint32_t heap_size);


#endif //LIYUX_OS_X86_64_MALLOC_H
