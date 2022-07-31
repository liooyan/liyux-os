#include <kernel/interrupt.h>
#include "kernel/tty.h"

void isr0() {
    tty_put_str("div 0\n");
}

void isr61() {
    tty_put_str("isr61\n");
}

void isr60() {
    tty_put_str("isr60\n");
}