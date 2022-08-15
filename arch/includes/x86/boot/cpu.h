//
// Created by root on 22-7-29.
//

#ifndef LIYUX_OS_CPU_H
#define LIYUX_OS_CPU_H

#include "stdint.h"

static inline uint32_t cpu_rcr0()
{
    uint32_t val;
    asm volatile("movl %%cr0,%0" : "=r"(val));
    return val;
}

static inline uint32_t cpu_rcr2()
{
    uint32_t val;
    asm volatile("movl %%cr2,%0" : "=r"(val));
    return val;
}

static inline uint32_t cpu_rcr3()
{
    uint32_t val;
    asm volatile("movl %%cr3,%0" : "=r"(val));
    return val;
}

static inline uint32_t cpu_rcr4()
{
    uint32_t val;
    asm volatile("movl %%cr4,%0" : "=r"(val));
    return val;
}



static inline void cpu_lcr0(uint32_t v)
{
    asm("mov %0, %%cr0" ::"r"(v));
}

static inline void cpu_lcr2(uint32_t v)
{
    asm("mov %0, %%cr2" ::"r"(v));
}

static inline void cpu_lcr3(uint32_t v)
{
    asm("mov %0, %%cr3" ::"r"(v));
}


static inline void cpu_lcr4(uint32_t v)
{
    asm("mov %0, %%cr4" ::"r"(v));
}


inline void cpu_lidt(uint32_t *index)
{
    asm("lidt %0" ::"m"(*index));
}

inline void cpu_lgdt(uint32_t *index)
{
    asm("lgdt %0" ::"m"(*index));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
            : "=a"(ret)
            : "Nd"(port) );
    return ret;
}

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint32_t inl(uint16_t port)
{
    uint32_t ret;
    asm volatile ( "inl %1, %0"
            : "=a"(ret)
            : "Nd"(port) );
    return ret;
}

static inline void outl(uint16_t port, uint32_t val)
{
    asm volatile ( "outl %0, %1" : : "a"(val), "Nd"(port) );
}


static inline void io_wait(void)
{
    outb(0x80, 0);
}




#define sti() __asm__ ("sti"::)
#endif //LIYUX_OS_CPU_H
