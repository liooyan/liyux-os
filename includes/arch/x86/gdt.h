//
// 关于gdt 的定义
// Created by lioyan.cn on 2022/7/26.
//
/**
 * 低32位
 * 0-15    段界限
 * 16-31   段基地址
 *
 * 高32位
 *
 * 0-7     段基地址
 * 8-11    TYPE
 * 12      S位 描述符类型 0 系统段、1数据段或代码段
 * 13-14   DPL
 * 15      P  present,表示当前段是否存在
 * 16-19   段基地址
 * 20      AVL 预留给操作系统使用(无用)
 * 21      L    64位代码段标志
 * 22      D/B
 * 23      G  gran 单位，1为4k
 * 24-31   段基地址
 */
#ifndef LIYUX_OS_GDT_H
#define LIYUX_OS_GDT_H
#include "stdint.h"

typedef struct  {
    uint16_t addr_limit_l;
    uint16_t addr_base_l;
    uint8_t addr_base_m;
    uint8_t type_dpl;
    uint8_t attr_addr_limit_h;
    uint8_t addr_base_h;
} gdt_descriptor_t;




// 设置不同位上的值
#define SET_S(x) (x << 4)
#define SET_DPL(x) (x << 5)
#define SET_P(x) (x << 7)

#define SET_AVL(x) (x)
#define SET_L(x) (x << 1)
#define SET_DB(x) (x << 2)
#define SET_G(x) (x << 3)

// 段基地址与段界限 设置
#define GET_LIM_L(x)            (x & 0x0ffff)
#define GET_LIM_H(x)            (x & 0xf0000 >> 16)
#define GET_BASE_L(x)           ((x & 0x0000ffff))
#define GET_BASE_M(x)           ((x & 0x00ff0000) >> 16)
#define GET_BASE_H(x)            (x & 0xff000000 >> 24)
#define GET_BASE(h,m,l)            (( h<< 24) |( m << 16) | l)

//TYPE 的所有枚举
#define TYPE_DATA_RD         0x00 // Read-Only
#define TYPE_DATA_RDA        0x01 // Read-Only, accessed
#define TYPE_DATA_RDWR       0x02 // Read/Write
#define TYPE_DATA_RDWRA      0x03 // Read/Write, accessed
#define TYPE_DATA_RDEXPD     0x04 // Read-Only, expand-down
#define TYPE_DATA_RDEXPDA    0x05 // Read-Only, expand-down, accessed
#define TYPE_DATA_RDWREXPD   0x06 // Read/Write, expand-down
#define TYPE_DATA_RDWREXPDA  0x07 // Read/Write, expand-down, accessed
#define TYPE_CODE_EX         0x08 // Execute-Only
#define TYPE_CODE_EXA        0x09 // Execute-Only, accessed
#define TYPE_CODE_EXRD       0x0A // Execute/Read
#define TYPE_CODE_EXRDA      0x0B // Execute/Read, accessed
#define TYPE_CODE_EXC        0x0C // Execute-Only, conforming
#define TYPE_CODE_EXCA       0x0D // Execute-Only, conforming, accessed
#define TYPE_CODE_EXRDC      0x0E // Execute/Read, conforming
#define TYPE_CODE_EXRDCA     0x0F // Execute/Read, conforming, accessed




#define GDT_R0_CODE         (TYPE_CODE_EXRD | SET_S(1) | SET_DPL(0) | SET_P(1))

#define GDT_R0_DATA         (TYPE_DATA_RDWR | SET_S(1) | SET_DPL(0) | SET_P(1))

#define GDT_DEF_ATTR        SET_AVL(0) | SET_L(0) | SET_DB(1) |   SET_G(1)


#define GDT_SELECTOR_CODE_GLOBAL 0x08
#define GDT_SELECTOR_DATA_GLOBAL 0x10


void _init_gdt();

void gdt_print(unsigned int index);

#endif //LIYUX_OS_GDT_H
