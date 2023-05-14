// devcpu.cpp - CPU Device Package
//
// Date:    May 8, 2023
// Author:  Tim Stark

#include "emu/core.h"
#include "emu/devcpu.h"

CPUDevice::CPUDevice(const SystemConfig &config, cDeviceType &type,
    cstr_t &devName, Device *owner, uint64_t clock)
: Device(config, type, devName, owner, clock)
{
}
