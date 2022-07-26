//
// Created by 李延 on 2022/7/25.
//

#include "kernel/tty.h"

int _init_main(){
    init_tty();
    for (int i = 0; i < 10000; ++i) {
        tty_put_str("test");
    }
    return 1;
}