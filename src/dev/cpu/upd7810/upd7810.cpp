// upd7810.h - NEC uPD7810 processor package
//
// Author:  Tim Stark
// Date:    Jan 9, 2022

#include "emu/core.h"
#include "emu/devcb.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "dev/cpu/upd7810/upd7810.h"

upd7810_cpuDevice::upd7810_cpuDevice(const SystemConfig &config, const DeviceType &type,
    cstag_t &devName, Device *owner, uint64_t clock)
: ProcessorDevice(config, type, devName, owner, clock),
  mapProgramConfig("program", LittleEndian, 8, 16, 2, 8, 16, 16, 4, 0)
{

}

map::AddressConfigList upd7810_cpuDevice::getAddressConfigList() const
{
    return map::AddressConfigList
    {
        { map::asProgram, &mapProgramConfig }
    };
}

void upd7810_cpuDevice::devStart()
{
    // Assign memory spaces
    getAddressSpace(map::asProgram)->setSpecificMemory(mapProgram);

}

void upd7810_cpuDevice::setData128(map::AddressList &map)
{
    map(0xFF80, 0xFFFF).ram();
}

void upd7810_cpuDevice::setData256(map::AddressList &map)
{
    map(0xFF00, 0xFFFF).ram();
}

// ********

DEFINE_DEVICE_TYPE(UPD7810, upd7810_cpuDevice, "uPD7810", "NEC uPD7810")

upd7810_cpuDevice::upd7810_cpuDevice(const SystemConfig &config,
    cstag_t &devName, Device *owner, uint64_t clock)
: upd7810_cpuDevice(config, UPD7810, devName, owner, clock)
{ }