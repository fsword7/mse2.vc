// devcpu.h - CPU processor device
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

#pragma once

#include "emu/diexec.h"
#include "emu/dimem.h"

class ProcessorDevice
: public Device,
  public diMemory,
  public diExecute
{
public:
    ProcessorDevice(const SystemConfig &config, const DeviceType &type,
        cstag_t &devName, Device *owner, uint64_t clock);

};

using cpuDevice = ProcessorDevice;
