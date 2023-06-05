// axpcpu.cpp - Alpha Processor package
//
// Author:  Tim Stark
// Date:    May 14, 2023

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "emu/devcpu.h"
#include "dev/cpu/alpha/axpcpu.h"

AlphaProcessor::AlphaProcessor(SystemConfig &config, cDeviceType &type,
    cstr_t &devName, Device *owner, uint64_t clock, int paWidth, int aWidth)
: CPUDevice(config, type, devName, owner, clock),
  mapProgramConfig("program", LittleEndian,  64, 16, 8, 8,  paWidth, 16, aWidth, 8)
{
    assert(paWidth == 34 || paWidth == 40 || paWidth == 44);
    
}

map::AddressConfigList AlphaProcessor::getAddressConfigList() const
{
    return map::AddressConfigList {
		{ map::asProgram, &mapProgramConfig }
	};
}