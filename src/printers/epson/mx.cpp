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
#include "printers/epson/mx.h"

void mx80_Device::mx80(SystemConfig &config)
{
    I8039(config, pcpu, "pcpu", 0);
    I8041A(config, scpu, "scpu", 0);

    // cpu->setAddressMap(map::asProgram, &vt100_Device::vt100_setMemoryMap);
    // cpu->setAddressMap(map::asIOPort, &vt100_Device::vt100_setIOPort);
}

void mx80_Device::mx100(SystemConfig &config)
{
    I8039(config, pcpu, "pcpu", 0);
    I8041A(config, scpu, "scpu", 0);

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
    FW_END      
};

static const fwEntry_t FW_NAME(mx100)[] =
{
    FW_END      
};

PRINTER(mx80, nullptr, epson, mx80, mx80_Device, mx80, mx80_init, "Epson", "MX80 Printer", SYSTEM_NOT_WORKING)
PRINTER(mx100, nullptr, epson, mx100, mx80_Device, mx100, mx100_init, "Epson", "MX100 Printer", SYSTEM_NOT_WORKING)