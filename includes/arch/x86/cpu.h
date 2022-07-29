//
// Created by root on 22-7-29.
//

#ifndef LIYUX_OS_CPU_H
#define LIYUX_OS_CPU_H
typedef unsigned int reg32;

static inline reg32 cpu_rcr0()
{
    uintptr_t val;
    asm volatile("movl %%cr0,%0" : "=r"(val));
    return val;
}

static inline reg32 cpu_rcr2()
{
    uintptr_t val;
    asm volatile("movl %%cr2,%0" : "=r"(val));
    return val;
}

static inline reg32 cpu_rcr3()
{
    uintptr_t val;
    asm volatile("movl %%cr3,%0" : "=r"(val));
    return val;
}


static inline void cpu_lcr0(reg32 v)
{
    asm("mov %0, %%cr0" ::"r"(v));
}

static inline void cpu_lcr2(reg32 v)
{
    asm("mov %0, %%cr2" ::"r"(v));
}

static inline void cpu_lcr3(reg32 v)
{
    asm("mov %0, %%cr3" ::"r"(v));
}

#endif //LIYUX_OS_CPU_H
