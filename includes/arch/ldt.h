//
// Created by root on 11/2/22.
//

#ifndef LIYUX_OS_LDT_H
#define LIYUX_OS_LDT_H
#include "arch/gdt.h"


#define LDT_R3_TYPE         (TYPE_LDT | SET_S(0) | SET_DPL(3) | SET_P(1))
#define LDT_R0_TYPE         (TYPE_LDT | SET_S(0) | SET_DPL(0) | SET_P(1))


#define TASK_R3_TYPE         (TYPE_TASK | SET_S(0) | SET_DPL(3) | SET_P(1))


#define GDT_LDT_ATTR        SET_AVL(0) | SET_L(0) | SET_DB(0) |   SET_G(0)


typedef struct  {
    u16 offset_1;        // 低16位偏移地址
    u16 base_address_1;        //低16位基地址
    u8 base_address_2;   //中8位基地址
    u8 type_dpl;         //定义高32位中，8-15位的信息
    u8 attr_offset_2;   //定义高32位中，16-23位的信息
    u8 base_address_3;        //高8位基地址
} ldt_descriptor_t;


typedef union {
    u32 ldt_u32[2];
    ldt_descriptor_t ldt_descriptor;
    task_descriptor_t task_descriptor;
} ldt_t;

u32 register_ldt_in_gdt(ldt_t *ldt, u32 ldt_size);

u32 register_ldt_entry( ldt_t *ldt,u32 ldt_size,u32 base, u32 limit, u8 avl_attr, u8 type_dpl);

#endif //LIYUX_OS_LDT_H
