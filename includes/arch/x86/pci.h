//
// Created by Administrator on 2022/8/1.
//

#ifndef LIYUX_OS_PCI_H
#define LIYUX_OS_PCI_H

#define PCI_CONFIG_ADDR 0xcf8
#define PCI_CONFIG_DATA 0xcfc

#define PCI_DEV_VENDOR(x) ((x)&0xffff)
#define PCI_VENDOR_INVLD 0xffff
#define PCI_REG_BAR(num) (0x10 + (num) * 4)
#define PCI_BAR_MMIO(x) (!((x)&0x1))
#define PCI_BAR_ADDR_MM(x) ((x) & ~0xf)
#include "stdint.h"

typedef struct pci_index {
    uint8_t bus;
    uint8_t dev;
    uint8_t func;
} pci_index_t;


typedef struct pci_device {
    pci_index_t pci_index;
    uint16_t vendor_info;
    uint16_t device_info;
    uint32_t class_info;
    uint8_t irq;
    uint32_t bar[6];
} pci_device_t;

void _pci_probe();

pci_device_t *select_pci_by_class(uint32_t class);
int _pci_bar_sizing( pci_device_t* dev, uint32_t bar_num);

#endif //LIYUX_OS_PCI_H
