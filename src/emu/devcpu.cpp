// devcpu.cpp - CPU processor device
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

#include "emu/core.h"
#include "emu/devcpu.h"

ProcessorDevice::ProcessorDevice(const SystemConfig &config, const DeviceType &type,
    cstag_t &devName, Device *owner, uint64_t clock)
: Device(config, type, devName, owner, clock),
  diMemory(this), diExecute(this)
{
    
}