//
// Created by Administrator on 2022/8/12.
//

#ifndef LIYUX_OS_X86_64_MALLOC_H
#define LIYUX_OS_X86_64_MALLOC_H

#include "lib/int_ll32.h"

//分配堆内存，这是一个不可释放空间的临时堆内存，用于setup的引导程序
void * malloc(u32 size);
//分配4k对齐的空间
u32 *malloc_4k(u32 size);
void heap_init(u32 addr,u32 heap_size);


#endif //LIYUX_OS_X86_64_MALLOC_H
