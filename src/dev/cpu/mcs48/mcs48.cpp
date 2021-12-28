// mcs48.cpp - MCS 48 Processor series package
//
// Author:  Tim Stark
// Date:    12/28/21

#include "emu/core.h"
#include "emu/map/map.h"
#include "dev/cpu/mcs48/mcs48.h"

mcs48_cpuDevice::mcs48_cpuDevice(const SystemConfig &config, const DeviceType &type,
    cstag_t &devName, Device *owner, uint64_t clock, int paWidth, int daWidth)
: ProcessorDevice(config, type, devName, owner, clock),
  mapProgramConfig("program", LittleEndian, 8, 16, 2, 8, paWidth, 16, 3, 0),
  mapDataConfig("data", LittleEndian, 8, 16, 2, 8, daWidth, 16, 2, 0),
  mapIOPortConfig("I/O port", LittleEndian, 8, 16, 2, 8, 8, 16, 2, 0)
{

}

map::AddressConfigList mcs48_cpuDevice::getAddressConfigList() const
{
    return map::AddressConfigList
    {
        { map::asProgram, &mapProgramConfig },
        { map::asData,    &mapDataConfig    },
        { map::asIOPort,  &mapIOPortConfig  }
    };
}

DEFINE_DEVICE_TYPE(i8035, i8035_cpuDevice, "i8035", "Intel 8035")
DEFINE_DEVICE_TYPE(i8039, i8039_cpuDevice, "i8039", "Intel 8039")
DEFINE_DEVICE_TYPE(i8040, i8040_cpuDevice, "i8040", "Intel 8039")
DEFINE_DEVICE_TYPE(i8048, i8048_cpuDevice, "i8048", "Intel 8048")
DEFINE_DEVICE_TYPE(i8049, i8049_cpuDevice, "i8049", "Intel 8049")
DEFINE_DEVICE_TYPE(i8050, i8050_cpuDevice, "i8050", "Intel 8050")
DEFINE_DEVICE_TYPE(i8749, i8749_cpuDevice, "i8749", "Intel 8749")
DEFINE_DEVICE_TYPE(i8750, i8750_cpuDevice, "i8750", "Intel 8750")

// 8035 with external ROM and 64 bytes RAM
i8035_cpuDevice::i8035_cpuDevice(const SystemConfig &config,
	cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, i8035, devName, owner, clock, 10, 6)
{
}

// 8039 with external ROM and 128 bytes RAM
i8039_cpuDevice::i8039_cpuDevice(const SystemConfig &config,
	cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, i8039, devName, owner, clock, 11, 7)
{
}

// 8040 with external ROM and 256 bytes RAM
i8040_cpuDevice::i8040_cpuDevice(const SystemConfig &config,
	cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, i8040, devName, owner, clock, 12, 8)
{
}

// 8048 with 1K ROM and 64 bytes RAM
i8048_cpuDevice::i8048_cpuDevice(const SystemConfig &config,
	cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, i8048, devName, owner, clock, 10, 6)
{
}

// 8049 with 2K ROM and 128 bytes RAM
i8049_cpuDevice::i8049_cpuDevice(const SystemConfig &config,
	cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, i8049, devName, owner, clock, 11, 7)
{
}

// 8050 with 4K ROM and 256 bytes RAM
i8050_cpuDevice::i8050_cpuDevice(const SystemConfig &config,
	cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, i8050, devName, owner, clock, 12, 8)
{
}

// 8749 with 2K EPROM and 128 bytes RAM
i8749_cpuDevice::i8749_cpuDevice(const SystemConfig &config,
	cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, i8749, devName, owner, clock, 11, 7)
{
}

// 8750 with 4K EPROM and 256 bytes RAM
i8750_cpuDevice::i8750_cpuDevice(const SystemConfig &config,
	cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, i8750, devName, owner, clock, 12, 8)
{
}
