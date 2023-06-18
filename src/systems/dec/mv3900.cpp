// mv3900.cpp - MicroVAX 3900 system package
//
// Date:    May 11, 2023
// Author:  Tim Stark

#include "emu/core.h"
#include "emu/devsys.h"

class mv3900_Device : public SystemDevice
{
public:
    mv3900_Device(SystemConfig &config, const DeviceType &type, cstr_t &devName, uint64_t clock)
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
static const fwEntry_t FW_NAME(mv3900)[] =
{      
    // FW_REGION("cpu", 0x10000, FW_ERASEFF),
    // FW_LOAD("23-061e2-00.e56", 0x0000, 0x0800, 0, nullptr),
    // FW_LOAD("23-032e2-00.e52", 0x0800, 0x0800, 0, nullptr),
    // FW_LOAD("23-033e2-00.e45", 0x1000, 0x0800, 0, nullptr),
    // FW_LOAD("23-034e2-00.e40", 0x1800, 0x0800, 0, nullptr),

	FW_END
};

static const fwEntry_t FW_NAME(vs3900)[] =
{      
    // FW_REGION("cpu", 0x10000, FW_ERASEFF),
    // FW_LOAD("23-061e2-00.e56", 0x0000, 0x0800, 0, nullptr),
    // FW_LOAD("23-032e2-00.e52", 0x0800, 0x0800, 0, nullptr),
    // FW_LOAD("23-033e2-00.e45", 0x1000, 0x0800, 0, nullptr),
    // FW_LOAD("23-034e2-00.e40", 0x1800, 0x0800, 0, nullptr),

	FW_END
};

COMPUTER(mv3900, nullptr, dec, mv3900, mv3900_Device, mv3900, mv3900_init, "DEC", "MicroVAX 3900", SYSTEM_NOT_WORKING);
COMPUTER(vs3900, nullptr, dec, mv3900, mv3900_Device, vs3900, vs3900_init, "DEC", "VAXserver 3900", SYSTEM_NOT_WORKING);
