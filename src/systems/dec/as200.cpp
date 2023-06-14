// as200.cpp - AlphaStation 200/400 package
//
// Author:  Tim Stark
// Date:    May 14, 2023

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "emu/devsys.h"
#include "emu/devcpu.h"

#include "dev/cpu/alpha/axpcpu.h"
#include "dev/cpu/alpha/ev4cpu.h"

class as200_Device : public SystemDevice
{
public:
    as200_Device(SystemConfig &config, const DeviceType &type, cstr_t &devName, uint64_t clock)
    : SystemDevice(config, type, devName, clock),
      cpu(*this, "cpu"),
      ramData(*this, "ram")
    //   crt(*this, "VT100_Video"),
    //   usart(*this, "usart"),
    //   dbrg(*this, "dbrg"),
    //   rs232(*this, "rs232"),
    //   nvr(*this, "nvr"),
    //   ramData(*this, "ram")
    {

    }

    void as200(SystemConfig &config);
    void as200_init();

    void as400(SystemConfig &config);
    void as400_init();

    // Memory configuration routines
    void as200_setProgram(map::AddressList &map);
    void as400_setProgram(map::AddressList &map);

private:
    RequiredDevice<axp21064_cpuDevice> cpu;
    // RequiredDevice<vt100video_t> crt;
    // RequiredDevice<i8251_Device> usart;
    // RequiredDevice<com5016_013_Device> dbrg;
    // RequiredDevice<rs232_portDevice> rs232;
    // RequiredDevice<er1400_Device> nvr;

    RequiredRegionPointer<uint8_t> ramData;
};


void as200_Device::as200(SystemConfig &config)
{
    AXP21064(config, cpu, "cpu", 0);
    cpu->setAddressMap(map::asProgram, &as200_Device::as200_setProgram);

}

void as200_Device::as200_init()
{

}

void as200_Device::as200_setProgram(map::AddressList &map)
{
    map.setUnmappedHigh();
    map(0x0000'0000, 0x17FF'FFFF).ram().allocate(0x1000'0000).region("ram");
}

void as200_Device::as400(SystemConfig &config)
{
    AXP21064(config, cpu, "cpu", 0);
    cpu->setAddressMap(map::asProgram, &as200_Device::as400_setProgram);

}

void as200_Device::as400_init()
{

}

void as200_Device::as400_setProgram(map::AddressList &map)
{
    map.setUnmappedHigh();
    map(0x0000'0000, 0x17FF'FFFF).ram().allocate(0x1000'0000).region("ram");
}

// cchar_t as200_memList[] =
// {
//     { "8m",   8 * 1024 * 1024 },
//     { "16m",  16 * 1024 * 1024 },
//     { "32m",  32 * 1024 * 1024 },
//     { "64m",  64 * 1024 * 1024 },
//     { "128m", 128 * 1024 * 1024 },
//     { "256m", 256 * 1024 * 1024 },
//     { "384m", 384 * 1024 * 1024 }
// };
 
COMPUTER(as200, nullptr, dec, as200, as200_Device, as200, as200_init, "DEC", "AlphaStation 200", SYSTEM_NOT_WORKING);
COMPUTER(as400, nullptr, dec, as200, as200_Device, as400, as400_init, "DEC", "AlphaStation 400", SYSTEM_NOT_WORKING);
