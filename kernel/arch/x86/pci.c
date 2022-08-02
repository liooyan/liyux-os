//
// Created by Administrator on 2022/8/1.
//
#include "stdint.h"
#include "arch/x86/cpu.h"
#include "arch/x86/pci.h"


pci_device_t pci_device[256];

static void installation_pci(pci_device_t *pci){
    for (int i = 0; i < 256; ++i) {
        if(pci_device[i].device_info == 0){
            pci_device[i].device_info = pci->device_info;
            pci_device[i].vendor_info = pci->vendor_info;
            pci_device[i].class_info = pci->class_info;
            pci_device[i].irq = pci->irq;
            return;
        }
    }

}

/**
 * 读取pic总线数据
 *
 * @param bus
 * @param slot
 * @param func
 * @param offset
 * @return
 */
static uint32_t pic_config_read(uint8_t bus, uint8_t dev, uint8_t func, uint8_t offset) {
    uint32_t address = ((uint32_t) 0x80000000) | (bus << 16) | (dev << 11) |
                       (func << 8) | (offset & 0xFC);

    outl(PCI_CONFIG_ADDR, address);

    return inl(PCI_CONFIG_DATA);
}

static void pic_probe_device(uint8_t bus, uint8_t dev, uint8_t func) {

    //读取到1号寄存器
    uint32_t reg1 = pic_config_read(bus, dev, func, 0);
    if (PCI_DEV_VENDOR(reg1) == PCI_VENDOR_INVLD) {
        return;
    }

    uint32_t hdr_type = pic_config_read(bus, dev, func, 0xc);
    hdr_type = (hdr_type >> 16) & 0xff;
    // 探测多用途设备
    if ((hdr_type & 0x80) && func == 0) {
        for (int func = 1; func < 7; func++) {
            pic_probe_device(bus, dev, func);
        }
    }

    uint32_t intr = pic_config_read(bus, dev, func, 0x3c);
    uint32_t class = pic_config_read(bus, dev, func,  0x8);
    pci_device_t pci = {
            .class_info =class ,
            .device_info=reg1 >> 16,
            .vendor_info = reg1 & 0xffff,
            .irq = intr & 0xff
    };
    installation_pci(&pci);

}


void _pci_probe() {

    for (uint8_t bus_num = 0; bus_num < 32; ++bus_num) {
        for (uint8_t dev_num = 0; dev_num < 32; ++dev_num) {
            pic_probe_device(bus_num, dev_num, 0);
        }
    }
}


