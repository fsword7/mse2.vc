// alpha.cpp - AXP Processor package
//
// Author:  Tim Stark
// Date:    May 14, 2023

#include "emu/core.h"
#include "emu/devcpu.h"
#include "dev/cpu/alpha/axp.h"

AlphaProcessor::AlphaProcessor(const SystemConfig &config, cDeviceType &type,
    cstr_t &devName, Device *owner, uint64_t clock)
: CPUDevice(config, type, devName, owner, clock)
{
}
