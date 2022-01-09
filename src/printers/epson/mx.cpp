// mx80.cpp - Epson MX80/MX100 dot matrix printer
//
// Author:  Tim Stark
// Date:    12/28/21

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "emu/devsys.h"
#include "emu/devcpu.h"

#include "dev/cpu/mcs48/mcs48.h"
#include "dev/chip/i8155.h"
#include "printers/epson/mx.h"

// P1 I/O flag definitions

// P2 I/O flag definitions
#define P2_IOM      0x20    // 8155 IO/M flag (0 = memory, 1 = I/O)
#define P2_8155     0x08    // 8155 access enable
#define P2_8041     0x04    // 8041 access enable
#define P2_DIP2     0x02    // DIP2 access enable
#define P2_DIP1     0X01    // DIP1 access enable

// DIP1 switch definitions

// DIP2 switch definitions


uint8_t mx80_Device::read8io(offs_t offset)
{
    // if ((p2Flags & P2_8155) == 0)
    // {
    //     i8155.select(p2Flags & P2_IOM);
    //     return i8155.read8(offset);
    // }
    if ((p2Flags & P2_DIP1) == 0)
        return sw1Flags;
    if ((p2Flags & P2_DIP2) == 0)
        return sw2Flags;
    return 0;
}

void mx80_Device::write8io(offs_t offset, uint8_t data)
{
    // if ((p2Flags & P2_8155) == 0)
    // {
    //     i8155.select(p2Flags & P2_IOM);
    //     i8155.write8(offset, data);
    // }
}

void mx80_Device::mx80(SystemConfig &config)
{
    I8039(config, pmcu, "pmcu", 0);
    pmcu->setAddressMap(map::asProgram, &mx80_Device::mx80_setProgram);
    pmcu->setAddressMap(map::asIOPort, &mx80_Device::mx80_setExtData);

    I8041A(config, smcu, "smcu", 0);

    I8155(config, i8155, "ramio", 0);

    // cpu->setAddressMap(map::asProgram, &vt100_Device::vt100_setMemoryMap);
    // cpu->setAddressMap(map::asIOPort, &vt100_Device::vt100_setIOPort);

    sw1Flags = 0;
    sw2Flags = 0;
}

void mx80_Device::mx100(SystemConfig &config)
{
    I8039(config, pmcu, "pmcu", 0);
    I8041A(config, smcu, "smcu", 0);
    I8155(config, i8155, "ramio", 0);

    // cpu->setAddressMap(map::asProgram, &vt100_Device::vt100_setMemoryMap);
    // cpu->setAddressMap(map::asIOPort, &vt100_Device::vt100_setIOPort);

    sw1Flags = 0;
    sw2Flags = 0;
}

void mx80_Device::mx80_init()
{

}

void mx80_Device::mx100_init()
{

}

void mx80_Device::mx80_setProgram(map::AddressList &map)
{
    // map(0x800, 0x7FF).bankr("mx80fw");
    map(0x000, 0x7FF).rom().region("mx80fw");
    map(0x800, 0xFFF).rom().region("mx80fw", 0x1000);
}

// 8039/8049 external data access
void mx80_Device::mx80_setExtData(map::AddressList &map)
{
    map.setUnmappedHigh();
    // map(0x00, 0xFF).rw(FUNC(mx80_Device::read8io), FUNC(mx80_Device::write8io));
}


static const fwEntry_t FW_NAME(mx80)[] =
{
    FW_REGION("mx80fw", 0x1800, 0),
    FW_LOAD("a2ha3.1b.bin", 0x0000, 0x0800, 0, nullptr),
    FW_LOAD("a1ha2.2b.bin", 0x0800, 0x0800, 0, nullptr),
    FW_LOAD("a2ha1.3b.bin", 0x1000, 0x0800, 0, nullptr),

    FW_REGION("smcu", 0x0400, 0),
    FW_LOAD("d8041c.9b.bin", 0x0000, 0x0400, 0, nullptr),

    FW_END      
};

static const fwEntry_t FW_NAME(mx100)[] =
{
    FW_END      
};

PRINTER(mx80, nullptr, epson, mx80, mx80_Device, mx80, mx80_init, "Epson", "MX80 Printer", SYSTEM_NOT_WORKING)
PRINTER(mx100, nullptr, epson, mx100, mx80_Device, mx100, mx100_init, "Epson", "MX100 Printer", SYSTEM_NOT_WORKING)