// mcs80.cpp - Intel 8080/8085 Processor package
//
// Author:  Tim Stark
// Date:    Dec 10, 2021

#include "emu/core.h"
#include "emu/map/map.h"
#include "dev/cpu/mcs80/mcs80.h"

mcs80_cpuDevice::mcs80_cpuDevice(const SystemConfig &config, const DeviceType &type,
    cstag_t &devName, Device *owner, uint64_t clock, cpuType idType)
: ProcessorDevice(config, type, devName, owner, clock),
  mapProgramConfig("program", LittleEndian, 8, 16, 2, 8,  16, 16, 4, 0),
  mapIOPortConfig("I/O", LittleEndian, 8, 16, 2, 8,  8, 16, 2, 0),
  idType(idType)
{
}

map::AddressConfigList mcs80_cpuDevice::getAddressConfigList() const
{
    return map::AddressConfigList
    {
        { map::asProgram, &mapProgramConfig },
        { map::asIOPort,  &mapIOPortConfig  }
    };
}

void mcs80_cpuDevice::devStart()
{
    // Assign memory spaces
    getAddressSpace(map::asProgram)->setSpecificMemory(mapProgram);
    getAddressSpace(map::asIOPort)->setSpecificMemory(mapIOPort);

    initOpcodeTable();

    setCycleCounter(&cpuCycles);
    
    // Initializing MCS80/MCS95 processor
    init();
    reset();
}

DEFINE_DEVICE_TYPE(I8080, mcs80_cpuDevice, "i8080", "Intel 8080")
DEFINE_DEVICE_TYPE(I8080A, mcs80a_cpuDevice, "i8080A", "Intel 8080A")
DEFINE_DEVICE_TYPE(I8085, mcs85_cpuDevice, "i8085", "Intel 8085")


// Intel 8080 processor
mcs80_cpuDevice::mcs80_cpuDevice(const SystemConfig &config,
    cstag_t &devName, Device *owner, uint64_t clock)
: mcs80_cpuDevice(config, I8080, devName, owner, clock, cpuid_8080)
{
}

// Intel 8080A processor
mcs80a_cpuDevice::mcs80a_cpuDevice(const SystemConfig &config,
    cstag_t &devName, Device *owner, uint64_t clock)
: mcs80_cpuDevice(config, I8080A, devName, owner, clock, cpuid_8080A)
{
}

// Intel 8085 processor
mcs85_cpuDevice::mcs85_cpuDevice(const SystemConfig &config,
    cstag_t &devName, Device *owner, uint64_t clock)
: mcs80_cpuDevice(config, I8085, devName, owner, clock, cpuid_8085)
{
}