#include <kernel/interrupt.h>

#include "kernel/tty/tty.h"
#include "arch/x86/pic.h"
#include "arch/x86/cpu.h"



void isr0() {
    tty_put_str("div 0\n");
}

void keyword_event() {
 //   tty_put_str("isr81\n");
    uint8_t  a = inb(0x60);
//    tty_put_str(a);
    pic_send_end(KEYWORD_EVENT_INDEX);
}

void timer_event(){
    static int i = 0;
 //   tty_put_str("timer event\n");
    pic_send_end(KEYWORD_EVENT_INDEX);
}