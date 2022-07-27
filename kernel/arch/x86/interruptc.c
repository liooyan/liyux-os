#include <kernel/interrupt.h>
#include "kernel/tty.h"

void isr0() {
    tty_put_str("div 0\n");
}
