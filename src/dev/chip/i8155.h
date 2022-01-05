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

protected:
    i8155_Device(const SystemConfig &config, const DeviceType &type, cstag_t &devName, Device *owner, uint64_t clock);

};

class i8156_Device : public i8155_Device
{
public:
    i8156_Device(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
 
};

DECLARE_DEVICE_TYPE(I8155, i8155_Device)
DECLARE_DEVICE_TYPE(I8156, i8156_Device)