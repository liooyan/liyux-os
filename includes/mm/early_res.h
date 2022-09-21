//
// Created by root on 9/21/22.
//

#ifndef LIYUX_OS_EARLY_RES_H
#define LIYUX_OS_EARLY_RES_H
#include "liyux/int_ll64.h"
#include "stdint.h"

#define MAX_EARLY_RES_X 500

struct early_res {
    uint64_t start, end;
    char name[15];
    char overlap_ok;
};
#endif //LIYUX_OS_EARLY_RES_H
