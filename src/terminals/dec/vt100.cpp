// vt100.cpp - DEC VT100 terminal emulator
//
// Author:  Tim Stark
// Date:    12/8/2021

#include "emu/core.h"
#include "emu/map/addrmap.h"
#include "emu/devsys.h"
#include "emu/devcpu.h"

#include "dev/cpu/i8080/i8080.h"
#include "terminals/dec/vt100.h"

void vt100_Device::vt100(SystemConfig &config)
{
    // fmt::printf("VT100 device configuration here.\n");

    i8080(config, cpu, "cpu", 0);
}

void vt100_Device::vt100_init()
{

}

void vt100_Device::vt100_setMemoryMap(aspace::AddressList &map)
{
    map.setUnmappedHigh();
    map(0x0000, 0x1FFF).rom().region("vt100fw");
    map(0x2000, 0x3FFF).ram().share("ram");
    map(0x8000, 0x9FFF).rom();
    map(0xA000, 0xBFFF).rom();
}

void vt100_Device::vt100_setIOPort(aspace::AddressList &map)
{
    map.setUnmappedHigh();
}

TERMINAL(vt100, nullptr, dec, vt100, vt100_Device, vt100, vt100_init, "DEC", "VT100 Terminal", SYSTEM_NOT_WORKING)