// m6502.h - MOS 6502 microprocessor package
//
// Author:  Tim Stark
// Date:    Jan 16, 2022

#include "emu/core.h"
#include "dev/cpu/m6500/m6502.h"

m6502_cpuDevice::m6502_cpuDevice(const SystemConfig &config, const DeviceType &type,
    cstag_t &devName, Device *owner, uint64_t clock)
: ProcessorDevice(config, type, devName, owner, clock),
  mapProgramConfig("program", LittleEndian, 8, 16, 2, 8,  16, 16, 4, 0)
{
}

map::AddressConfigList m6502_cpuDevice::getAddressConfigList() const
{
    return map::AddressConfigList
    {
        { map::asProgram, &mapProgramConfig },
    };
}

void m6502_cpuDevice::devStart()
{
    // Assign memory spaces
    getAddressSpace(map::asProgram)->setSpecificMemory(mapProgram);

    // initOpcodeTable();

    setCycleCounter(&cpuCycles);
}

DEFINE_DEVICE_TYPE(M6502, m6502_cpuDevice, "6502", "MOS 6502")

// MOS 6502 processor
m6502_cpuDevice::m6502_cpuDevice(const SystemConfig &config,
    cstag_t &devName, Device *owner, uint64_t clock)
: m6502_cpuDevice(config, M6502, devName, owner, clock)
{
}
