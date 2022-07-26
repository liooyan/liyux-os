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


// 设置不同位上的值
#define SET_TYPE(x) (x << 8) 
#define SET_S(x) (x << 12) 
#define SET_DPL(x) (x << 13) 
#define SET_P(x) (x << 15) 
#define SET_AVL(x) (x << 20) 
#define SET_L(x) (x << 21) 
#define SET_DB(x) (x << 22) 
#define SET_G(x) (x << 23)

// 段基地址与段界限 设置
#define SET_LIM_L(x)            (x & 0x0ffff)
#define SET_LIM_H(x)            (x & 0xf0000)
#define SET_BASE_L(x)           ((x & 0x0000ffff) << 16)
#define SET_BASE_M(x)           ((x & 0x00ff0000) >> 16)
#define SET_BASE_H(x)            (x & 0xff000000)


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



#define GDT_R0_CODE         SET_TYPE(TYPE_CODE_EXRD) | SET_S(1) | SET_DPL(0) | \
                            SET_P(1) | SET_AVL(0) | SET_L(0) | SET_DB(1) | \
                            SET_G(1)

#define GDT_R0_DATA         SET_TYPE(TYPE_DATA_RDWR) | SET_S(1) | SET_DPL(0) | \
                            SET_P(1) | SET_AVL(0) | SET_L(0) | SET_DB(1) | \
                            SET_G(1)


void _init_gdt();

#endif //LIYUX_OS_GDT_H
