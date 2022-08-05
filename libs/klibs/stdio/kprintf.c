//
// Created by Administrator on 2022/8/5.
//
#include "kernel/tty/tty.h"
#include "klib/format.h"
#include <stdarg.h>
#include <stdint.h>

void kprintf(const char *str, ...) {
    va_list args;
    va_start(args, str);
    while ((*str)) {
        if (*str == '/' && *(str + 1) == '%') {
            tty_put_chr(*(str + 1));
            str += 2;
            continue;
        }

        //开始转义
        if (*str == '%') {
            str++;
            format_info_t format_info;
            format_init(str, &format_info);
            //判断需要读取的格式
            if (format_info.specifier == FORMAT_STR) {
                char *value = va_arg(args, char *);
                while ((*value)) {
                    tty_put_chr(*value);
                    value++;
                }
            } else if (format_info.specifier == FORMAT_CHAR) {
                int value = va_arg(args, int);
                tty_put_chr(value);
            } else {
                char value[format_info.size];
                if (format_info.length != 2) {
                    int32_t src = va_arg(args, int32_t);
                    format(src, value, &format_info);
                } else {
                    int64_t src = va_arg(args, int64_t);
                    format(src, value, &format_info);
                }
                int i = 0;
                while (value[i]) {
                    tty_put_chr(value[i]);
                    i++;
                }
            }
            str += format_info.format_size;
        } else{
            tty_put_chr(*str);
        }
        str++;

    }
    va_end(args);

}