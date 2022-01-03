// mcs48.cpp - MCS 48 Processor series package
//
// Author:  Tim Stark
// Date:    12/28/21

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "dev/cpu/mcs48/mcs48.h"

mcs48_cpuDevice::mcs48_cpuDevice(const SystemConfig &config, const DeviceType &type,
    cstag_t &devName, Device *owner, uint64_t clock, int paWidth, int daWidth, int romSize)
: ProcessorDevice(config, type, devName, owner, clock),
  mapProgramConfig("program", LittleEndian, 8, 16, 2, 8, paWidth, 16, 3, 0),
  mapDataConfig("data", LittleEndian, 8, 16, 2, 8, daWidth, 16, 2, 0),
  mapIOPortConfig("I/O port", LittleEndian, 8, 16, 2, 8, 8, 16, 2, 0),
//   idata(*this, "data"),
  iromSize(romSize), iramSize(1u << daWidth)
{
    assert(paWidth == 11 || paWidth == 12);
    assert(daWidth == 6 || daWidth == 7 || daWidth == 8);
    assert(romSize == 0 || romSize == 1024 || romSize == 2048 || romSize == 4096);

    switch(romSize)
    {
        case 1024:
            mapProgramConfig.setAddressMap(map::Constructor(FUNC(mcs48_cpuDevice::setProgram1024), this));
            break;

        case 2048:
            mapProgramConfig.setAddressMap(map::Constructor(FUNC(mcs48_cpuDevice::setProgram2048), this));
            break;
 
        case 4096:
            mapProgramConfig.setAddressMap(map::Constructor(FUNC(mcs48_cpuDevice::setProgram4096), this));
            break;
    }

    switch (iramSize)
    {
        case 64:
            mapDataConfig.setAddressMap(map::Constructor(FUNC(mcs48_cpuDevice::setData64), this));
            break;

        case 128:
            mapDataConfig.setAddressMap(map::Constructor(FUNC(mcs48_cpuDevice::setData128), this));
            break;

        case 256:
            mapDataConfig.setAddressMap(map::Constructor(FUNC(mcs48_cpuDevice::setData256), this));
            break;
    }
}

map::AddressConfigList mcs48_cpuDevice::getAddressConfigList() const
{
    if (cpuFlags & CPUF_EXTERNAL_BUS)
    {
        return map::AddressConfigList
        {
            { map::asProgram, &mapProgramConfig },
            { map::asData,    &mapDataConfig    },
            { map::asIOPort,  &mapIOPortConfig  }
        };
    }
    else
    {
        return map::AddressConfigList
        {
            { map::asProgram, &mapProgramConfig },
            { map::asData,    &mapDataConfig    }
        };
    }
}

void mcs48_cpuDevice::devStart()
{
    // Assign memory spaces
    getAddressSpace(map::asProgram)->setSpecificMemory(mapProgram);
    getAddressSpace(map::asData)->setSpecificMemory(mapData);
    getAddressSpace(map::asIOPort)->setSpecificMemory(mapIOPort);
}

void mcs48_cpuDevice::setProgram1024(map::AddressList &map)
{
    map(0x000, 0x3FF).rom();
}

void mcs48_cpuDevice::setProgram2048(map::AddressList &map)
{
    map(0x000, 0x7FF).rom();
}

void mcs48_cpuDevice::setProgram4096(map::AddressList &map)
{
    map(0x000, 0xFFF).rom();
}

void mcs48_cpuDevice::setData64(map::AddressList &map)
{
    map(0x00, 0x3F).ram().share("data");
}

void mcs48_cpuDevice::setData128(map::AddressList &map)
{
    map(0x00, 0x7F).ram().share("data");
}

void mcs48_cpuDevice::setData256(map::AddressList &map)
{
    map(0x00, 0xFF).ram().share("data");
}

DEFINE_DEVICE_TYPE(I8021, i8021_cpuDevice, "i8021", "Intel 8021")
DEFINE_DEVICE_TYPE(I8022, i8022_cpuDevice, "i8022", "Intel 8022")
DEFINE_DEVICE_TYPE(I8035, i8035_cpuDevice, "i8035", "Intel 8035")
DEFINE_DEVICE_TYPE(I8039, i8039_cpuDevice, "i8039", "Intel 8039")
DEFINE_DEVICE_TYPE(I8040, i8040_cpuDevice, "i8040", "Intel 8039")
DEFINE_DEVICE_TYPE(I8048, i8048_cpuDevice, "i8048", "Intel 8048")
DEFINE_DEVICE_TYPE(I8648, i8648_cpuDevice, "i8648", "Intel 8648")
DEFINE_DEVICE_TYPE(I8748, i8748_cpuDevice, "i8748", "Intel 8748")
DEFINE_DEVICE_TYPE(I8049, i8049_cpuDevice, "i8049", "Intel 8049")
DEFINE_DEVICE_TYPE(I8749, i8749_cpuDevice, "i8749", "Intel 8749")
DEFINE_DEVICE_TYPE(I8050, i8050_cpuDevice, "i8050", "Intel 8050")
DEFINE_DEVICE_TYPE(I8750, i8750_cpuDevice, "i8750", "Intel 8750")

DEFINE_DEVICE_TYPE(I8041A, i8041a_cpuDevice, "i8041A", "Intel 8041A")
DEFINE_DEVICE_TYPE(I8741A, i8741a_cpuDevice, "i8741A", "Intel 8741A")
DEFINE_DEVICE_TYPE(I8041AH, i8041ah_cpuDevice, "i8041AH", "Intel 8041AH")
DEFINE_DEVICE_TYPE(I8741AH, i8741ah_cpuDevice, "i8741AH", "Intel 8741AH")
DEFINE_DEVICE_TYPE(I8042, i8042_cpuDevice, "i8042", "Intel 8042")
DEFINE_DEVICE_TYPE(I8742, i8742_cpuDevice, "i8742", "Intel 8742")
DEFINE_DEVICE_TYPE(I8042AH, i8042ah_cpuDevice, "i8042AH", "Intel 8042AH")
DEFINE_DEVICE_TYPE(I8742AH, i8742ah_cpuDevice, "i8742AH", "Intel 8742AH")

i8021_cpuDevice::i8021_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, I8035, devName, owner, clock, 12, 6, 1024)
{ }

i8022_cpuDevice::i8022_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, I8035, devName, owner, clock, 12, 7, 2048)
{ }

i8035_cpuDevice::i8035_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, I8035, devName, owner, clock, 12, 6, 0)
{ }

i8039_cpuDevice::i8039_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, I8039, devName, owner, clock, 12, 7, 0)
{ }

i8040_cpuDevice::i8040_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, I8040, devName, owner, clock, 12, 8, 0)
{ }

i8048_cpuDevice::i8048_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, I8048, devName, owner, clock, 12, 6, 1024)
{ }

i8648_cpuDevice::i8648_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, I8648, devName, owner, clock, 12, 6, 1024)
{ }

i8748_cpuDevice::i8748_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, I8748, devName, owner, clock, 12, 6, 1024)
{ }

i8049_cpuDevice::i8049_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, I8049, devName, owner, clock, 12, 7, 2048)
{ }

i8749_cpuDevice::i8749_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, I8749, devName, owner, clock, 12, 7, 2048)
{ }

i8050_cpuDevice::i8050_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, I8050, devName, owner, clock, 12, 8, 4096)
{ }

i8750_cpuDevice::i8750_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, I8750, devName, owner, clock, 12, 8, 4096)
{ }


// Intel UPI-41 microcontroller series

i8041a_cpuDevice::i8041a_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: upi41_cpuDevice(config, I8041A, devName, owner, clock, 11, 6, 1024)
{ }

i8741a_cpuDevice::i8741a_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: upi41_cpuDevice(config, I8741A, devName, owner, clock, 11, 6, 1024)
{ }

i8041ah_cpuDevice::i8041ah_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: upi41_cpuDevice(config, I8041AH, devName, owner, clock, 11, 7, 1024)
{ }

i8741ah_cpuDevice::i8741ah_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: upi41_cpuDevice(config, I8741AH, devName, owner, clock, 11, 7, 1024)
{ }

i8042_cpuDevice::i8042_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: upi41_cpuDevice(config, I8042, devName, owner, clock, 11, 7, 2048)
{ }

i8742_cpuDevice::i8742_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: upi41_cpuDevice(config, I8742, devName, owner, clock, 11, 7, 2048)
{ }

i8042ah_cpuDevice::i8042ah_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: upi41_cpuDevice(config, I8042AH, devName, owner, clock, 11, 8, 2048)
{ }

i8742ah_cpuDevice::i8742ah_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: upi41_cpuDevice(config, I8742AH, devName, owner, clock, 11, 8, 2048)
{ }
