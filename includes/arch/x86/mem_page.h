//
// Created by Administrator on 2022/7/29.
//

/***
 * 页目录与页表格式如下：
 * 0: P  是 存在 位， 为“ 1” 时， 表示 页 表 或者 页 位于 内存 中
 * RW: 是 读/ 写 位。 为“ 0” 时 表示 这样 的 页 只能 读取， 为“ 1” 时， 可读 可 写。
 * US: 是 用户/ 管理 位。 为“ 1” 时， 允许 所有 特权 级别 的 程序 访问； 为“ 0” 时， 只 允许 特权 级别 为 0、 1 和 2 的 程序 访问， 特权 级别 为 3 的 程序 不能 访问。
 *
 *
 */
#ifndef LIYUX_OS_MEM_PAGE_H
#define LIYUX_OS_MEM_PAGE_H

#include <stdint.h>
#include <arch/x86/rlevel.h>


#define MEM_PAGE_ATTR(RW, MP_US, P) P | RW << 1 | MP_US << 2

#define MEM_GET_PAGE_ADDR(ADD) (ADD & 0xfffff000)

#define MEM_PAGE(ADD,RW, MP_US, P) MEM_GET_PAGE_ADDR(ADD) | MEM_PAGE_ATTR(RW, MP_US, P)




//读写权限
#define MP_RW_R 0
#define MP_RW_W 1

#define MP_US_R0 0
#define MP_US_RALL 1

#define MP_P_HAVE 1
#define MP_P_NONE 0


#define PAGE_SIZE 1024


void _inti_page(uint32_t *kpg);


#endif //LIYUX_OS_MEM_PAGE_H
