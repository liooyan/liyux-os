//
// Created by root on 11/9/22.
//

#ifndef LIYUX_OS_APIC_H
#define LIYUX_OS_APIC_H


#define  IA32_APIC_BASE_MSR 0x1B
#define  IA32_APIC_OPEN(MSR_LOW)  (MSR_LOW | (1 << 11))

#define  APIC_OPEN(tpr)  (tpr | (1 << 8))

#define  APIC_ID 0X20

#define  APIC_TPR 0XF0
#define  APIC_EOI 0XB0


#define  PIC_ADDRESS_1 0X21
#define  PIC_ADDRESS_2 0Xa1
#define  PIC_CLOSE_VAL 0xff



void _init_apic();

void apic_send_end();

#endif //LIYUX_OS_APIC_H
