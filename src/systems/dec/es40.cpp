// es40.cpp - AlphaServer ES40 package
//
// Author:  Tim Stark
// Date:    May 14, 2023

#include "emu/core.h"
#include "emu/devsys.h"

class es40_Device : public SystemDevice
{
public:
    es40_Device(SystemConfig &config, const DeviceType &type, cstr_t &devName, uint64_t clock)
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

    void es40(SystemConfig &config);
    void es40_init();

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


void es40_Device::es40(SystemConfig &config)
{

}

void es40_Device::es40_init()
{

}

static const fwEntry_t FW_NAME(es40)[] =
{      
    // FW_REGION("cpu", 0x10000, FW_ERASEFF),
    // FW_LOAD("23-061e2-00.e56", 0x0000, 0x0800, 0, nullptr),
    // FW_LOAD("23-032e2-00.e52", 0x0800, 0x0800, 0, nullptr),
    // FW_LOAD("23-033e2-00.e45", 0x1000, 0x0800, 0, nullptr),
    // FW_LOAD("23-034e2-00.e40", 0x1800, 0x0800, 0, nullptr),

	FW_END
};


COMPUTER(es40, nullptr, dec, es40, es40_Device, es40, es40_init, "DEC", "AlphaServer ES40", SYSTEM_NOT_WORKING);
