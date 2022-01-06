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

void mx80_Device::mx80(SystemConfig &config)
{
    I8039(config, pmcu, "pmcu", 0);
    I8041A(config, smcu, "smcu", 0);

    // cpu->setAddressMap(map::asProgram, &vt100_Device::vt100_setMemoryMap);
    // cpu->setAddressMap(map::asIOPort, &vt100_Device::vt100_setIOPort);
}

void mx80_Device::mx100(SystemConfig &config)
{
    I8039(config, pmcu, "pmcu", 0);
    I8041A(config, smcu, "smcu", 0);

    // cpu->setAddressMap(map::asProgram, &vt100_Device::vt100_setMemoryMap);
    // cpu->setAddressMap(map::asIOPort, &vt100_Device::vt100_setIOPort);
}

void mx80_Device::mx80_init()
{

}

void mx80_Device::mx100_init()
{

}

void mx80_Device::mx80_setMemoryMap(map::AddressList &map)
{
}

static const fwEntry_t FW_NAME(mx80)[] =
{
    FW_REGION("mx80fw", 0x1800, 0),
    FW_LOAD("a1ha2.2b.bin", 0x0000, 0x0800, 0, nullptr),
    FW_LOAD("a2ha1.1b.bin", 0x0800, 0x0800, 0, nullptr),
    FW_LOAD("a2ha3.3b.bin", 0x1000, 0x0800, 0, nullptr),

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