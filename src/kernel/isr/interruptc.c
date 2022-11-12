#include <kernel/isr/interrupt.h>
#include "lib/stdio.h"
#include "arch/apic.h"


void isr0() {
    kprintf("div 0\n");
}
void isr_time() {
    kprintf("timer\n");
    apic_send_end();
}
