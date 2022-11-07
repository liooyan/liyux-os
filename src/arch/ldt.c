//
// Created by root on 10/29/22.
//


#include "arch/gdt.h"
#include "arch/ldt.h"
#include "lib/int_ll32.h"

u32 find_free_index(ldt_t *ldt, u32 ldt_size);

u32 register_ldt_in_gdt(ldt_t *ldt, u32 ldt_size) {
    return   register_gdt_entry((u32)ldt,ldt_size,GDT_LDT_ATTR,LDT_R0_TYPE);
}

u32 register_ldt_entry( ldt_t *ldt, u32 ldt_size,u32 base, u32 limit, u8 avl_attr, u8 type_dpl) {
    u32 index =find_free_index(ldt,ldt_size);
    ldt = ldt+index;
    ldt->ldt_descriptor.base_address_1 = GET_BASE_L(base);
    ldt->ldt_descriptor.base_address_2 = GET_BASE_M(base);
    ldt->ldt_descriptor.base_address_3 = GET_BASE_H(base);
    ldt->ldt_descriptor.offset_1 = GET_LIM_L(limit);
    ldt->ldt_descriptor.attr_offset_2 = avl_attr << 4 |GET_LIM_H(limit);
    ldt->ldt_descriptor.type_dpl = type_dpl;
    return index << 3 | 0B100 | GET_DPL(type_dpl);
}




u32 find_free_index(ldt_t *ldt, u32 ldt_size){
    for (u32 i = 0; i < ldt_size; ++i){
        ldt_t *ldt_next = ldt+i;
        if(ldt_next->ldt_u32[0] == 0 && ldt_next->ldt_u32[1] == 0){
            return i;
        }
    }
    return -1;
}