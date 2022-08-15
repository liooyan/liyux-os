//
// Created by Administrator on 2022/8/12.
//

#ifndef LIYUX_OS_X86_64_MALLOC_H
#define LIYUX_OS_X86_64_MALLOC_H
#include "stdint.h"
//分配堆内存，这是一个不可释放空间的临时堆内存，用于setup的引导程序
void * malloc(uint32_t size);
//分配4k对齐的空间
uint64_t *malloc_4k(uint32_t size);
void heap_init(uint32_t addr,uint32_t heap_size);

extern uint32_t malloc_addr;
extern uint32_t malloc_heap_size;

#endif //LIYUX_OS_X86_64_MALLOC_H
