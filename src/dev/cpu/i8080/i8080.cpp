// i8080.cpp - Intel 8080/8085 Processor package
//
// Author:  Tim Stark
// Date:    12/10/2021

#include "emu/core.h"
#include "emu/devcpu.h"
#include "dev/cpu/i8080/i8080.h"

i8080_cpuDevice::i8080_cpuDevice(const SystemConfig &config, const DeviceType &type,
    cstag_t &devName, Device *owner, uint64_t clock)
: ProcessorDevice(config, type, devName, owner, clock),
  mapProgramConfig("program", LittleEndian, 8, 16, 2, 8,  16, 16, 4, 0),
  mapIOPortConfig("I/O", LittleEndian, 8, 16, 1, 8,  8, 16, 2, 0)
{
}

map::AddressConfigList i8080_cpuDevice::getAddressConfigList() const
{
    return map::AddressConfigList
    {
        { map::asProgram, &mapProgramConfig },
        { map::asIOPort,  &mapIOPortConfig  }
    };
}

DEFINE_DEVICE_TYPE(i8080, i8080_cpuDevice, "i8080", "Intel 8080")
DEFINE_DEVICE_TYPE(i8080a, i8080a_cpuDevice, "i8080A", "Intel 8080A")
DEFINE_DEVICE_TYPE(i8085, i8085_cpuDevice, "i8085", "Intel 8085")


// Intel 8080 processor
i8080_cpuDevice::i8080_cpuDevice(const SystemConfig &config,
    cstag_t &devName, Device *owner, uint64_t clock)
: i8080_cpuDevice(config, i8080, devName, owner, clock)
{
}

// Intel 8080A processor
i8080a_cpuDevice::i8080a_cpuDevice(const SystemConfig &config,
    cstag_t &devName, Device *owner, uint64_t clock)
: i8080_cpuDevice(config, i8080a, devName, owner, clock)
{
}

// Intel 8085 processor
i8085_cpuDevice::i8085_cpuDevice(const SystemConfig &config,
    cstag_t &devName, Device *owner, uint64_t clock)
: i8080_cpuDevice(config, i8085, devName, owner, clock)
{
}