//
// Created by root on 10/29/22.
//

#ifndef LIYUX_OS_IDT_H
#define LIYUX_OS_IDT_H
#include "lib/int_ll32.h"

// 段基地址与段界限 设置
#define GET_LIM_L(x)            (x & 0x0ffff)
#define GET_LIM_H(x)            (x & 0xf0000 >> 16)
#define GET_BASE_L(x)           ((x & 0x0000ffff))
#define GET_BASE_M(x)           ((x & 0x00ff0000) >> 16)
#define GET_BASE_H(x)            ((x & 0xff000000) >> 24)
#define GET_BASE(h,m,l)            (( h<< 24) |( m << 16) | l)



#define GET_CALL_BASE_L(x)           ((x & 0x0000ffff))
#define GET_CALL_BASE_H(x)           ((x & 0xffff0000) >> 16)



// 设置不同位上的值
#define SET_S(x) (x << 4)
#define SET_DPL(x) (x << 5)
#define SET_P(x) (x << 7)

#define SET_AVL(x) (x)
#define SET_L(x) (x << 1)
#define SET_DB(x) (x << 2)
#define SET_G(x) (x << 3)


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

#define TYPE_TSS     0B1001 //

#define GDT_R0_CODE         (TYPE_CODE_EXRD | SET_S(1) | SET_DPL(0) | SET_P(1))
#define GDT_R3_CODE         (TYPE_CODE_EXRD | SET_S(1) | SET_DPL(3) | SET_P(1))

#define GDT_R0_DATA         (TYPE_DATA_RDWR | SET_S(1) | SET_DPL(0) | SET_P(1))
#define GDT_R3_DATA         (TYPE_DATA_RDWR | SET_S(1) | SET_DPL(3) | SET_P(1))

#define GDT_DEF_ATTR        SET_AVL(0) | SET_L(0) | SET_DB(1) |   SET_G(1)

#define GDT_TSS_ATTR        SET_AVL(0) | SET_L(0) | SET_DB(0) |   SET_G(1)
#define TSS_R3_TYPE         (TYPE_TSS | SET_S(0) | SET_DPL(3) | SET_P(1))
#define TSS_R0_TYPE         (TYPE_TSS | SET_S(0) | SET_DPL(0) | SET_P(1))

typedef struct  {
    u16 offset_1;        // 低16位偏移地址
    u16 base_address_1;        //低16位基地址
    u8 base_address_2;   //中8位基地址
    u8 type_dpl;         //定义高32位中，8-15位的信息
    u8 attr_offset_2;   //定义高32位中，16-23位的信息
    u8 base_address_3;        //高8位基地址
} gdt_descriptor_t;


typedef struct  {
    u16 offset_1;        // 低16位偏移地址
    u16 selector;        //低16位基地址
    u8 param_count;   //中8位基地址
    u8 attr;         //定义高32位中，8-15位的信息
    u16 offset_2;        //高8位基地址
} call_descriptor_t;


typedef union {
    gdt_descriptor_t gdt_descriptor;
    call_descriptor_t call_descriptor;
} gdt_t;



typedef struct {
    u16 _gdt_limit;
    gdt_t *gdt;
} __attribute__((packed)) gdt_index_t;



typedef struct  {
    u16 last_tss;
    u16 keep_1;
    u32 esp0;
    u16 ss0;
    u16 keep_2;
    u32 esp1;
    u16 ss1;
    u16 keep_3;
    u32 esp2;
    u16 ss2;
    u16 keep_4;
    u32 cr3;

    u32 eip;
    u32 eflags;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;
    u16 es;
    u16 keep_5;
    u16 cs;
    u16 keep_6;
    u16 ss;
    u16 keep_7;
    u16 ds;
    u16 keep_8;
    u16 fs;
    u16 keep_9;
    u16 gs;
    u16 keep_10;
    u16 ldt;
    u16 keep_11;
    u16 keep_12;
    u16 io_map;
} tss_t;






void set_gdt_entry(u32 index, u32 base, u32 limit, u8 avl_attr, u8 type_dpl);



void load_gdt();
#endif //LIYUX_OS_IDT_H
