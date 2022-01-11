// lx810.cpp - Epson LX810/AP2000 dot matrix printer
//
// Author:  Tim Stark
// Date:    Jan 9, 2022

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

    uint8_t fmData = 0; // fake memory data

    // Fake memory function calls
    void write8fm(uint8_t data) { fmData = data; }
    uint8_t read8fm()           { return fmData; }
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
    map(0x0000, 0x7FFF).rom().region("ap2000fw");
    map(0x8000, 0x9FFF).ram();
    map(0xA000, 0xBFFF).rw(FUNC(ap2000_Device::read8fm), FUNC(ap2000_Device::write8fm));
    // map(0xC000, 0xC00F).mirror(0x1FF0)
    map(0xE000, 0xFEFF).noprw(); // not used
    // 0xFF00-0xFFFF - reserveed for uPD7810 internal RAM space
}

static const fwEntry_t FW_NAME(ap2000)[] =
{
    FW_REGION("ap2000fw", 0x8000, 0),
    FW_LOAD("ap2k.ic3c", 0x0000, 0x8000, 0, nullptr),

    // FW_REGION("eeprom", 0x20),
    // FW_LOAD("at93c06", 0x00, 0x20),

    FW_END      
};

// PRINTER(lx810l, nullptr, epson, ap2000, ap2000_Device, ap2000, ap2000_init, "Epson", "Epson LX810L Printer", SYSTEM_NOT_WORKING)
PRINTER(ap2000, nullptr, epson, ap2000, ap2000_Device, ap2000, ap2000_init, "Epson", "Epson AP2000 Printer", SYSTEM_NOT_WORKING)
