//
// Created by root on 22-7-29.
//

#ifndef LIYUX_OS_CPU_H
#define LIYUX_OS_CPU_H


#include "lib/int_ll32.h"


inline void cpu_lgdt(const u32 *index) {
    asm("lgdt %0"::"m"(*index));
}


inline void cpu_lcr0(u16 v) {
    asm("mov %0, %%ax \nmov %%ax, %%cr0"::"r"(v));
}


inline void cpu_lidt(u32 *index) {
    asm("lidt %0"::"m"(*index));
}


inline void cpu_outb(u16 port,u8 val) {
    asm volatile ( "outb %%al, %0" : :  "N"(port) ,"a"(val));
}


inline void cpu_rdmsr(u32 address, u64 *ret) {
    asm("rdmsr "
            : "=d"(ret->u64_high), "=a"(ret->u64_low)
            :"c"(address));
}

inline void cpu_wrmsr(u32 address, u64 *data) {
    asm("wrmsr "
            :
            :"d"(data->u64_high), "a"(data->u64_low), "c"(address));
}


#endif //LIYUX_OS_CPU_H
