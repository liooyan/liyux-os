//
// Created by root on 22-7-30.
//
#include "arch/x86/pic.h"
#include "arch/x86/cpu.h"
#include "stdint.h"

#define HZ 100
#define LATCH (1193180/HZ)

/**
 * 发送pic处理完成指令
 * @param irq
 */
void pic_send_end(uint8_t irq){
    if(irq >= 8)
        outb(PIC2_COMMAND,PIC_EOI);

    outb(PIC1_COMMAND,PIC_EOI);
}

void _init_pic(int offset1, int offset2){

    inb(PIC1_DATA);                        // save masks
    inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
    outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
    outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)

    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    outb(PIC1_DATA, 0xfc);   // restore saved masks.
    outb(PIC2_DATA, 0xff);


    outb(0x43,0x36);		/* binary, mode 3, LSB/MSB, ch 0 */
    outb(0x40,LATCH & 0xff );	/* LSB */
    outb(0x40,LATCH >> 8 );	/* MSB */

}

void IRQ_set_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);
}

void IRQ_clear_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);
}