// mv3900.cpp - MicroVAX 3900 system package
//
// Date:    May 11, 2023
// Author:  Tim Stark

#include "emu/core.h"
#include "emu/devsys.h"

class mv3900_Device : public SystemDevice
{
public:
    mv3900_Device(const SystemConfig &config, const DeviceType &type, cstr_t &devName, uint64_t clock)
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

    void mv3900(SystemConfig &config);
    void mv3900_init();

    void vs3900(SystemConfig &config);
    void vs3900_init();

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


void mv3900_Device::mv3900(SystemConfig &config)
{

}

void mv3900_Device::vs3900(SystemConfig &config)
{

}

void mv3900_Device::mv3900_init()
{

}

void mv3900_Device::vs3900_init()
{

}

COMPUTER(mv3900, nullptr, dec, mv3900, mv3900_Device, mv3900, mv3900_init, "DEC", "MicroVAX 3900", SYSTEM_NOT_WORKING);
COMPUTER(vs3900, nullptr, dec, mv3900, mv3900_Device, vs3900, vs3900_init, "DEC", "VAXserver 3900", SYSTEM_NOT_WORKING);
