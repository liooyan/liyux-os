//
// Created by 李延 on 2022/7/25.
//

#include "kernel/tty.h"
#include "stdint.h"

//vga 显示的基地址
vga_attribute *TTY_BASH_ADDRESS = 0xB8000;

uint32_t TTY_COLUMN = 0;
uint32_t TTY_ROW = 0;


//通过一个全局变量，设置显示颜色
vga_attribute theme_color = VGA_COLOR_BLACK;

void tty_set_theme(tty_uint16_t fg, tty_uint16_t bg) {
    theme_color = (bg << 4 | fg) << 8;
}


void tty_put_chr(char chr) {
    *(TTY_BASH_ADDRESS + TTY_COLUMN + TTY_ROW * TTY_WIDTH) = (theme_color | chr);
    TTY_COLUMN++;
    check_width();
    check_height();


}


void tty_put_str(char *str) {
    while (*str != '\0') {
        tty_put_chr(*str);
        str++;
    }
}


void tty_clear() {

    for (int i = 0; i < TTY_WIDTH * TTY_HEIGHT; ++i) {
        *(TTY_BASH_ADDRESS + i) = theme_color;
    }


}


static void check_width() {
    if (TTY_COLUMN >= TTY_WIDTH) {
        TTY_ROW++;
        TTY_COLUMN = 0;
    }
}


static void check_height() {
    if (TTY_ROW >= TTY_HEIGHT) {
        tty_scroll_up();
    }
}
