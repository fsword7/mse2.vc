// as200.cpp - AlphaStation 200/400 package
//
// Author:  Tim Stark
// Date:    May 14, 2023

#include "emu/core.h"
#include "emu/devsys.h"

class as200_Device : public SystemDevice
{
public:
    as200_Device(const SystemConfig &config, const DeviceType &type, cstr_t &devName, uint64_t clock)
    : SystemDevice(config, type, devName, clock)
    //   cpu(*this, "i8080"),
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

    // void mv3900_setMemoryMap(map::AddressList &map);
    
private:
    // RequiredDevice<mcs80_cpuDevice> cpu;
    // RequiredDevice<vt100video_t> crt;
    // RequiredDevice<i8251_Device> usart;
    // RequiredDevice<com5016_013_Device> dbrg;
    // RequiredDevice<rs232_portDevice> rs232;
    // RequiredDevice<er1400_Device> nvr;

    // RequiredSharedPointer<uint8_t> ramData;
};


void as200_Device::as200(SystemConfig &config)
{

}

void as200_Device::as200_init()
{

}

void as200_Device::as400(SystemConfig &config)
{

}

void as200_Device::as400_init()
{

}

COMPUTER(as200, nullptr, dec, as200, as200_Device, as200, as200_init, "DEC", "AlphaStation 200", SYSTEM_NOT_WORKING);
COMPUTER(as400, nullptr, dec, as200, as200_Device, as400, as400_init, "DEC", "AlphaStation 400", SYSTEM_NOT_WORKING);
