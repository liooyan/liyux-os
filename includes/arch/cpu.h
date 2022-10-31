//
// Created by root on 22-7-29.
//

#ifndef LIYUX_OS_CPU_H
#define LIYUX_OS_CPU_H


#include "lib/int_ll32.h"

inline void cpu_lgdt(const u32 *index)
{
    asm("lgdt %0" ::"m"(*index));
}


static inline void cpu_lcr0(u16 v)
{
    asm("mov %0, %%ax \nmov %%ax, %%cr0" ::"r"(v));
}
#endif //LIYUX_OS_CPU_H