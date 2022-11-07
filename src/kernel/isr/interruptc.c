#include <kernel/isr/interrupt.h>
#include "lib/stdio.h"


void isr0() {
    kprintf("div 0\n");
}
