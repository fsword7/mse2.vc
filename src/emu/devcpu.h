// devcpu.h - CPU Device Package
//
// Date:    May 8, 2023
// Author:  Tim Stark

#pragma once

#include "emu/dimem.h"
#include "emu/diexec.h"
#include "emu/didebug.h"
class CPUDevice : public Device,
    public diMemory, public diExecute, public diDebug
{
public:
    CPUDevice(SystemConfig &config, cDeviceType &type,
        cstr_t &devName, Device *owner, uint64_t clock);

};