//
// Created by root on 11/9/22.
//
#include "arch/apic.h"
#include "arch/cpu.h"
#include "lib/int_ll32.h"

u32 apic_base_address;

static void set_apic_base_address(u64 *apic_msr){

    apic_base_address = apic_msr->u64_low & ~((1<< 12)-1);
}

static  inline void close_8259a_pic(){
    cpu_outb(PIC_ADDRESS_1,PIC_CLOSE_VAL);
    cpu_outb(PIC_ADDRESS_2,PIC_CLOSE_VAL);
}

static  inline void read_apic_id(u64 *apic_id){
    *apic_id = *(u64 *)(apic_base_address+APCI_ID_INDE);
}


void _init_apic(){

    u64 apic_msr ;
    cpu_rdmsr(IA32_APIC_BASE_MSR,&apic_msr);
    set_apic_base_address(&apic_msr);
    apic_msr.u64_low = apic_msr.u64_low |(1 << 10);
    cpu_wrmsr(IA32_APIC_BASE_MSR,&apic_msr);
    close_8259a_pic();
}