//
// Created by Administrator on 2022/7/29.
//
/**
 * 内存分布：
 *  1mb-14mb 为内核使用
 *  显示取 1mb前
 *
 *
 */

#include <arch/x86/mem_page.h>
#include "kernel/tty.h"
#include "arch/x86/cpu.h"

extern uint8_t __kernel_start;
extern uint8_t __kernel_end;

static void init_page_zero(uint32_t *mapping_addr) {


    // 初始化 kpg 全为0
    for (uint32_t i = 0; i < PAGE_SIZE; i++) {
        *(mapping_addr + i) = 0;
    }

}


/**
 * 设置一个可读写的，r0权限页
 */
static void set_page_rw_r0(uint32_t *mapping_addr, uint32_t physical_addr) {
    *mapping_addr = MEM_PAGE(physical_addr, MP_RW_W, MP_US_R0, MP_P_HAVE);
}

/**
 * 设置一个可读写的，r3权限页
 */
static void set_page_rw_r3(uint32_t *mapping_addr, uint32_t physical_addr) {
    *mapping_addr = MEM_PAGE(physical_addr, MP_RW_W, MP_US_RALL, MP_P_HAVE);
}

/**
 * 初始化一个页目录项
 * @param mapping_add
 * @param physical_add
 */
static void init_page_directory(uint32_t *page_directory_addr) {
    //先全部清零
    init_page_zero(page_directory_addr);
//    //将自己安装在最后一个页中
//    set_page_rw_r0((page_directory_addr + PAGE_SIZE - 1), page_directory_addr);

}

/**
 * 初始化一个页表
 *
 * @param page_directory_addr  页目录
 * @param page_table_addr  页表
 * @param physical_addr  物理地址
 */
static void init_page_table(uint32_t *page_directory_addr, uint32_t *page_table_addr, uint32_t index) {

    init_page_zero(page_table_addr);

    //将页表安装在页目录里
    set_page_rw_r0((page_directory_addr + index), page_table_addr);
//    //将自己安装在最后一页
//    set_page_rw_r0((page_table_addr + PAGE_SIZE - 1), page_table_addr);
//    //将页目录安装在对应的页表里
//    set_page_rw_r0((page_table_addr + index), page_directory_addr);
}

/**
 * 安装具体的某一个地址
 * @param page_directory_addr
 * @param page_table_addr
 * @param physical_addr 只传前20位
 */
static void init_page_addr(uint32_t *page_table_addr, uint32_t physical_addr) {

    uint32_t index = (0x3ff000 & physical_addr) >> 12;

    set_page_rw_r0((page_table_addr + index), physical_addr);
}


static int init_page_kernel(uint32_t *kpg) {
    //存放 内核
    int kernel_start_page_directory = __kernel_start >> 12;
    int kernel_end_page_directory = __kernel_end >> 12;
    int kernel_page_directory_size = kernel_end_page_directory - kernel_start_page_directory + 1;
    for (int i = 0; i < kernel_page_directory_size; i++) {
        uint32_t *page_table = kpg + PAGE_SIZE * (i + 1);
        init_page_table(kpg, page_table, kernel_start_page_directory);
        //映射具体的物理地址
        for (int j = 0; j < PAGE_SIZE; ++j) {
            init_page_addr(page_table, (((kernel_start_page_directory + i) << 10) + j) << 12);
        }
    }
    return kernel_page_directory_size;
}

static uint32_t init_vga_kernel(uint32_t *kpg, int vga_index) {
    uint32_t page_table = kpg[vga_index];
    set_page_rw_r0(((uint32_t *) MEM_GET_PAGE_ADDR(page_table)) + 1023, 0xB8000);


    return (vga_index << 22) + (1023 << 12);
}

void _inti_page(uint32_t *kpg) {

    //设置
    init_page_directory(kpg);
    int kernel_page_directory_size = init_page_kernel(kpg);

    //最后一页放vga
    uint32_t vga_bash = init_vga_kernel(kpg, kernel_page_directory_size - 1);
    _init_tty_bash_address((vga_attribute *) vga_bash);


    //开启内存分页
    cpu_lcr3(kpg);

    unsigned int cr0 = cpu_rcr0();
    cpu_lcr0(0x80010000 | cr0);

}
