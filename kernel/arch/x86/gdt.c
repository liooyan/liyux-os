//
// Created by lioyan.cn on 2022/7/26.
//
#include <arch/x86/gdt.h>
#include <stdint.h>
#include "klib/stdio.h"

#define GDT_ENTRY 6

gdt_descriptor_t _gdt[GDT_ENTRY];
uint16_t _gdt_limit = sizeof(_gdt) - 1;

static void set_gdt_entry(uint32_t index, uint32_t base, uint32_t limit, uint8_t attr, uint8_t type_dpl) {
    gdt_descriptor_t *gdtDescriptor = &_gdt[index];
    gdtDescriptor->addr_base_h = GET_BASE_H(base);
    gdtDescriptor->addr_base_m = GET_BASE_M(base);
    gdtDescriptor->addr_base_l = GET_BASE_L(base);
    gdtDescriptor->addr_limit_l = GET_LIM_L(limit);
    gdtDescriptor->attr_addr_limit_h = attr << 4 | GET_LIM_H(limit);
    gdtDescriptor->type_dpl = type_dpl;

}

static void set_r0_code_gdt(uint32_t index, uint32_t base, uint32_t limit) {
    set_gdt_entry(index, base, limit, GDT_DEF_ATTR, GDT_R0_CODE);
}

static void set_r0_data_gdt(uint32_t index, uint32_t base, uint32_t limit) {
    set_gdt_entry(index, base, limit, GDT_DEF_ATTR, GDT_R0_DATA);
}


void _init_gdt() {
    set_gdt_entry(0, 0, 0, 0, 0);
    set_r0_code_gdt(1, 0, 0xfffff);
    set_r0_data_gdt(2, 0, 0xfffff);

}

 void gdt_print(unsigned int index) {

    gdt_descriptor_t *gdtDescriptor = &_gdt[index];



    //判断类型为数据段或代码段
    if ((gdtDescriptor->type_dpl & (1 << 4)) != 0) {

        char *s_type;
        if ((gdtDescriptor->type_dpl & (1 << 3)) == 0) {
            s_type = "Data segment";
        } else {
            s_type = "Code segment";
        }

        index = (index << 3) ;
        uint32_t base_addr = GET_BASE(gdtDescriptor->addr_base_h, gdtDescriptor->addr_base_m,
                                      gdtDescriptor->addr_base_l);

        uint32_t limit = ((gdtDescriptor->attr_addr_limit_h & 0xff) << 16) | gdtDescriptor->addr_limit_l;
        //判断G
        if ((gdtDescriptor->attr_addr_limit_h & (1 << 7)) != 0) {
            limit = (limit << 12) | 0xfff;
        }

        uint8_t type = 0xf & gdtDescriptor->type_dpl;
        char *gdt_type="???";
        switch (type) {
            case TYPE_DATA_RD   :
                gdt_type = "Read-Only";
                break;
            case TYPE_DATA_RDA   :
                gdt_type = "Read-Only, accessed";
                break;
            case TYPE_DATA_RDWR      :
                gdt_type = "Read/Write";
                break;
            case TYPE_DATA_RDWRA     :
                gdt_type = "Read/Write, accessed";
                break;
            case TYPE_DATA_RDEXPD    :
                gdt_type = "Read-Only, expand-down";
                break;
            case TYPE_DATA_RDEXPDA   :
                gdt_type = "Read-Only, expand-down, accessed";
                break;
            case TYPE_DATA_RDWREXPD   :
                gdt_type = "Read/Write, expand-down";
                break;
            case TYPE_DATA_RDWREXPDA   :
                gdt_type = "Read/Write, expand-down, accessed";
                break;
            case TYPE_CODE_EX         :
                gdt_type = "Execute-Only";
                break;
            case TYPE_CODE_EXA        :
                gdt_type = "Execute-Only, accessed";
                break;
            case TYPE_CODE_EXRD        :
                gdt_type = "Execute/Read";
                break;
            case TYPE_CODE_EXRDA      :
                gdt_type = "Execute/Read, accessed";
                break;
            case TYPE_CODE_EXC        :
                gdt_type = "Execute-Only, conforming";
                break;
            case TYPE_CODE_EXCA       :
                gdt_type = "Execute-Only, conforming, accessed";
                break;
            case TYPE_CODE_EXRDC      :
                gdt_type = "Execute/Read, conforming";
                break;
            case TYPE_CODE_EXRDCA     :
                gdt_type = "Execute/Read, conforming, accessed";
                break;

        }


        kprintf("GDT[0x%4x]=%s,base=0x%8x,limit=0x%8x,%s\n",
                index,
                s_type,
                base_addr,
                limit,
                gdt_type
        );
    }

}