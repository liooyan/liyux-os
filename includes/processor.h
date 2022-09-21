//
// Created by root on 9/14/22.
//

#ifndef LIYUX_OS_PROCESSOR_H
#define LIYUX_OS_PROCESSOR_H

static inline void native_cpuid(unsigned int *eax, unsigned int *ebx,
                                unsigned int *ecx, unsigned int *edx) {
    asm volatile("cpuid"
            : "=a" (*eax),
    "=b" (*ebx),
    "=c" (*ecx),
    "=d" (*edx)
            : "0" (*eax), "2" (*ecx));
}


static inline void cpuid(unsigned int op,
                         unsigned int *eax, unsigned int *ebx,
                         unsigned int *ecx, unsigned int *edx) {
    *eax = op;
    *ecx = 0;
    native_cpuid(eax, ebx, ecx, edx);
}


#endif //LIYUX_OS_PROCESSOR_H
