//
// Created by lioyan.cn on 2022/7/25.
//

#ifndef LIYUX_OS_TTY_H
#define LIYUX_OS_TTY_H



typedef unsigned short vga_attribute;

#define VGA_COLOR_BLACK 0
#define VGA_COLOR_BLUE 1
#define VGA_COLOR_GREEN 2
#define VGA_COLOR_CYAN 3
#define VGA_COLOR_RED 4
#define VGA_COLOR_MAGENTA 5
#define VGA_COLOR_BROWN 6
#define VGA_COLOR_DARK_GREY 8
#define VGA_COLOR_LIGHT_GREY 7
#define VGA_COLOR_LIGHT_BLUE 9
#define VGA_COLOR_LIGHT_GREEN 10
#define VGA_COLOR_LIGHT_CYAN 11
#define VGA_COLOR_LIGHT_RED 12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_BROWN 14
#define VGA_COLOR_WHITE 15



void _init_tty(int width,int  height);
void _init_tty_bash_address(vga_attribute *addr);
void tty_set_theme(vga_attribute fg, vga_attribute bg);

void tty_put_chr(char chr);


void tty_put_str(char *str);

void tty_scroll_up();

void tty_clear();




#endif //LIYUX_OS_TTY_H
