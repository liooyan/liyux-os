//
// Created by Administrator on 2022/8/5.
//

#ifndef LIYUX_OS_STRING_H
#define LIYUX_OS_STRING_H
#include <stddef.h>



size_t strlen(const char* str);

size_t strnlen(const char* str, size_t max_len);

int streq(const char *source,const char *target);

void strcopy(char *source,char *target);

#endif //LIYUX_OS_STRING_H
