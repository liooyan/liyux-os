//
// Created by Administrator on 2022/8/5.
//
/**
 * 格式化，将字节格式化为指定的格式
 *
 */
#ifndef LIYUX_OS_FORMAT_H
#define LIYUX_OS_FORMAT_H

//以十进制形式输出带符号整数(正数不输出符号)
#define FORMAT_INT_BASE_10 'd'
//以八进制形式输出无符号整数(不输出前缀0)
#define FORMAT_UINT_BASE_8 'o'
//以十六进制形式输出无符号整数(不输出前缀Ox)
#define FORMAT_UINT_BASE_16 'x'
//	以十进制形式输出无符号整数
#define FORMAT_UINT_BASE_10 'u'

#define FORMAT_STR 's'

#define FORMAT_CHAR 'c'

#define FORMAT_LENGTH_TAG 'l'

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint32_t size; //格式化后字符串长度
    char specifier; //输出格式
    uint32_t length; //输出格式前面是否有l ，1为 一个l，2为2个l
    uint32_t width; //要输出的字符的最小数目。如果输出的值短于该数，结果会用空格填充。如果输出的值长于该数，结果不会被截断。
    size_t format_size; //format 所占字符;
} format_info_t;

void format_init(const char *format,format_info_t *format_info);

size_t format(int64_t src, char *results, const format_info_t *format_info);




#endif //LIYUX_OS_FORMAT_H
