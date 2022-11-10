//
// Created by root on 9/21/22.
//

#ifndef LIYUX_OS_INT_LL32_H
#define LIYUX_OS_INT_LL32_H


typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef struct {
    u32 u64_low;
    u32 u64_high;
} u64;

typedef struct {
    u32 u128_1;
    u32 u128_2;
    u32 u128_3;
    u32 u128_4;
} u128;

#endif //LIYUX_OS_INT_LL32_H
