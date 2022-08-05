//
// Created by 李延 on 2022/7/25.
//

#include "kernel/tty/tty.h"

//vga 显示的基地址
vga_attribute *TTY_BASH_ADDRESS ;

int TTY_COLUMN;
int TTY_ROW;

int TTY_WIDTH ;
int TTY_HEIGHT ;

//通过一个全局变量，设置显示颜色
vga_attribute theme_color = VGA_COLOR_BLACK;


static void check_width();


static void check_height();


void _init_tty_bash_address(vga_attribute *addr){
    //设置vga显示地址
    TTY_BASH_ADDRESS =addr;
}
void _init_tty(int width,int height){
    TTY_WIDTH = width;
    TTY_HEIGHT = height;
    //设置颜色
    tty_set_theme(VGA_COLOR_WHITE,VGA_COLOR_BLACK);
    //清屏
    tty_clear();
}


void tty_set_theme(vga_attribute fg, vga_attribute bg) {
    theme_color = (bg << 4 | fg) << 8;
}


void tty_put_chr(char chr) {
    //换行
    if(chr == '\n'){
        TTY_ROW++;
        TTY_COLUMN = 0;
    } else{
        *(TTY_BASH_ADDRESS + TTY_COLUMN + TTY_ROW * TTY_WIDTH) = (theme_color | chr);
        TTY_COLUMN++;
    }
    check_width();
    check_height();


}

void tty_put_buffer(char *str,int str_len) {
    for (int i = 0; i <str_len ; ++i) {
        tty_put_chr(str[i]);
    }
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

void tty_scroll_up() {
    //每行内容等于下一行内容
    for (int i = 0; i < TTY_HEIGHT - 1; ++i) {
        for (int j = 0; j < TTY_WIDTH; ++j) {
            *(TTY_BASH_ADDRESS + j + i * TTY_WIDTH) = *(TTY_BASH_ADDRESS + j + (i + 1) * TTY_WIDTH);
        }
    }

    //最后一行清空
    for (int i = 0; i < TTY_WIDTH; ++i) {
        *(TTY_BASH_ADDRESS + i + (TTY_HEIGHT - 1) * TTY_WIDTH) = theme_color;
    }
    //行数减少一行
    TTY_ROW --;


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
