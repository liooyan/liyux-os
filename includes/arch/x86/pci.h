//
// Created by Administrator on 2022/8/1.
//

#ifndef LIYUX_OS_PCI_H
#define LIYUX_OS_PCI_H

#define PCI_CONFIG_ADDR 0xcf8
#define PCI_CONFIG_DATA 0xcfc

#define PCI_DEV_VENDOR(x) ((x)&0xffff)
#define PCI_VENDOR_INVLD 0xffff

typedef struct pci_device
{
    uint16_t vendor_info;
    uint16_t device_info;
    uint32_t class_info;
    uint8_t irq;
} pci_device_t;

void _pci_probe();
#endif //LIYUX_OS_PCI_H
