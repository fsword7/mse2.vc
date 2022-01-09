// ap2000.cpp - Epson ActionPrinter AP2000 dot matrix printer
//
// Author:  Tim Stark
// Date:    1/9/22

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "emu/devsys.h"
#include "emu/devcpu.h"

#include "dev/cpu/upd7810/upd7810.h"

class ap2000_Device : public SystemDevice
{
public:
    ap2000_Device(const SystemConfig &config, const DeviceType &type, cstag_t &devName, uint64_t clock)
    : SystemDevice(config, type, devName, clock),
      cpu(*this, "cpu")
    {

    }

    void ap2000(SystemConfig &config);
    void ap2000_init();

    void ap2000_setProgram(map::AddressList &map);

private:
    RequiredDevice<upd7810_cpuDevice> cpu;
};

void ap2000_Device::ap2000(SystemConfig &config)
{
    UPD7810(config, cpu, "cpu", 0);
    cpu->setAddressMap(map::asProgram, &ap2000_Device::ap2000_setProgram);

}

void ap2000_Device::ap2000_init()
{
}

void ap2000_Device::ap2000_setProgram(map::AddressList &map)
{
}

static const fwEntry_t FW_NAME(ap2000)[] =
{
    FW_END      
};

PRINTER(ap2000, nullptr, epson, ap2000, ap2000_Device, ap2000, ap2000_init, "Epson", "ActionPrinter AP2000 Printer", SYSTEM_NOT_WORKING)
