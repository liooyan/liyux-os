//
// Created by Administrator on 2022/8/5.
//
#include "arch/x86/pci.h"
#include "arch/x86/ahci.h"
#include "kernel/tty.h"




void  _ahci_init(){
    pci_device_t *ahci_head = select_pci_by_class(AHCI_CLASS);
    uint32_t  sized  = _pci_bar_sizing(ahci_head,ABAR_INDEX);



    tty_put_chr(sized);

}

