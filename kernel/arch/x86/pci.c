//
// Created by Administrator on 2022/8/1.
//
#include "stdint.h"
#include "arch/x86/cpu.h"
#include "arch/x86/pci.h"


pci_device_t pci_device[256];

static void installation_pci(pci_device_t *pci) {
    for (int i = 0; i < 256; ++i) {
        if (pci_device[i].device_info == 0) {
            pci_device[i].device_info = pci->device_info;
            pci_device[i].vendor_info = pci->vendor_info;
            pci_device[i].class_info = pci->class_info;
            pci_device[i].irq = pci->irq;
            for (int j = 0; j < 6; ++j) {
                pci_device[i].bar[j] =pci->bar[j];
            }
            pci_device[i].pci_index.bus = pci->pci_index.bus;
            pci_device[i].pci_index.dev = pci->pci_index.dev;
            pci_device[i].pci_index.func = pci->pci_index.func;
            return;
        }
    }

}

static uint32_t  get_pci_pci_address(uint8_t bus, uint8_t dev, uint8_t func, uint8_t offset){
    return ((uint32_t) 0x80000000) | (bus << 16) | (dev << 11) |
                       (func << 8) | (offset & 0xFC);
}
static uint32_t pic_config_read(uint8_t bus, uint8_t dev, uint8_t func, uint8_t offset) {
    uint32_t address =get_pci_pci_address(bus,dev,func,offset);

    outl(PCI_CONFIG_ADDR, address);

    return inl(PCI_CONFIG_DATA);
}

static void  pic_config_write(uint8_t bus, uint8_t dev, uint8_t func, uint8_t offset, uint32_t value){
    uint32_t address =get_pci_pci_address(bus,dev,func,offset);
    outl(PCI_CONFIG_ADDR,address);
    outl(PCI_CONFIG_DATA, value);
}

 int _pci_bar_sizing( pci_device_t* dev, uint32_t bar_num)
{
    uint32_t  bar = dev->bar[bar_num];
    if (!bar) {
        return 0;
    }

    pic_config_write(dev->pci_index.bus,dev->pci_index.dev,dev->pci_index.func, PCI_REG_BAR(bar_num), 0xffffffff);
    uint32_t sized =
            pic_config_read(dev->pci_index.bus,dev->pci_index.dev,dev->pci_index.func, PCI_REG_BAR(bar_num)) & ~0x1;
    if (PCI_BAR_MMIO(bar)) {
        sized = PCI_BAR_ADDR_MM(sized);
    }
    pic_config_write(dev->pci_index.bus,dev->pci_index.dev,dev->pci_index.func, PCI_REG_BAR(bar_num), bar);
    return ~sized + 1;
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
    uint32_t class = pic_config_read(bus, dev, func, 0x8);

    pci_device_t pci = {
            .class_info =class,
            .device_info=reg1 >> 16,
            .vendor_info = reg1 & 0xffff,
            .irq = intr & 0xff,
            .pci_index = {
                    .bus = bus,
                    .dev = dev,
                    .func = func
            }
    };
    for (int i = 0; i < 6; ++i) {
        pci.bar[i] = pic_config_read(bus, dev, func, 0x10 + i * 4);
    }
    installation_pci(&pci);

}

pci_device_t *select_pci_by_class(uint32_t class) {
    for (int i = 0; i < 256; ++i) {
        if (pci_device[i].device_info == 0) {
            return &pci_device[i];
        }
        if ((pci_device[i].class_info >> 8 == class)) {
            return &pci_device[i];
        }
    }
    return &pci_device[255];

}


void _pci_probe() {

    for (uint8_t bus_num = 0; bus_num < 32; ++bus_num) {
        for (uint8_t dev_num = 0; dev_num < 32; ++dev_num) {
            pic_probe_device(bus_num, dev_num, 0);
        }
    }
}


