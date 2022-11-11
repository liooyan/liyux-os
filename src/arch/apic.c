//
// Created by root on 11/9/22.
//
#include <cpuid.h>
#include "arch/apic.h"
#include "arch/cpu.h"
#include "lib/int_ll32.h"
#include "lib/stdio.h"
#include "arch/timer.h"

u32 apic_base_address;

static u32 cpu_has_apic() {
    // reference: Intel manual, section 10.4.2
    u32 eax = 0, ebx = 0, edx = 0, ecx = 0;
    __get_cpuid(1, &eax, &ebx, &ecx, &edx);

    return (edx & 0x100);
}

static void set_apic_base_address(u64 *apic_msr) {

    apic_base_address = apic_msr->u64_low & ~((1 << 12) - 1);
}

static inline void close_8259a_pic() {
    cpu_outb(PIC_ADDRESS_1, PIC_CLOSE_VAL);
    cpu_outb(PIC_ADDRESS_2, PIC_CLOSE_VAL);
}

static inline u32 read_apic(u32 index) {
    return *(u32 *) (apic_base_address + index);
}

static inline void write_apic(u32 index, u32 val) {
    *(u32 *) (apic_base_address + index) = val;
}

static inline void open_apic() {
    u32 tpr = read_apic(APIC_TPR);
    tpr = tpr | (1 << 8);
    write_apic(APIC_TPR, 508);
}

void _init_time() {

    write_apic(APIC_LVT_TIME, 60);
    write_apic(APIC_TIME_DIV, 1024);
    write_apic(APIC_TIMER_ICR, 0x100000);

}

void _init_apic() {
    if (!cpu_has_apic()) {
        kprintf("no has apic\n");
        return;
    }
    close_8259a_pic();

    u64 apic_msr;
    cpu_rdmsr(IA32_APIC_BASE_MSR, &apic_msr);
    apic_msr.u64_low = apic_msr.u64_low | (1 << 11);
    cpu_wrmsr(IA32_APIC_BASE_MSR, &apic_msr);

    set_apic_base_address(&apic_msr);
    open_apic();
    _init_time();

}

