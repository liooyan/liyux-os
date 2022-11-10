//
// Created by root on 11/9/22.
//

#ifndef LIYUX_OS_APIC_H
#define LIYUX_OS_APIC_H


#define  IA32_APIC_BASE_MSR 0x1B

#define  APIC_ID 0X20

#define  APIC_TPR 0XF0



#define  PIC_ADDRESS_1 0X21
#define  PIC_ADDRESS_2 0Xa1
#define  PIC_CLOSE_VAL 0xff
void _init_apic();



#endif //LIYUX_OS_APIC_H
