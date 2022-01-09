// i8155.h - Intel 8155/8156 2048-bit static MOS RAM with I/O ports and timer
//
// Author:  Tim Stark
// Date:    Jan 4, 2022

#pragma once

#include "emu/devcb.h"

class i8155_Device : public Device
{
public:
    i8155_Device(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);

    void select(bool iomToggle);

    // I/O read/write accesses
    uint8_t read8io(offs_t offset);
    void write8io(offs_t offset, uint8_t data);

    // RAM read/write accsses
    uint8_t read8m(offs_t offset);
    void write8m(offs_t offset, uint8_t data);

    // read/wite acceses
    uint8_t read8(offs_t offset);
    void write8(offs_t offset, uint8_t data);

protected:
    i8155_Device(const SystemConfig &config, const DeviceType &type, cstag_t &devName, Device *owner, uint64_t clock);

private:
    bool iomSwitch = false;

    uint8_t idata[256];

};

class i8156_Device : public i8155_Device
{
public:
    i8156_Device(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
 
};

DECLARE_DEVICE_TYPE(I8155, i8155_Device)
DECLARE_DEVICE_TYPE(I8156, i8156_Device)