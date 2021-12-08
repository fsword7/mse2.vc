// vt100.cpp - DEC VT100 terminal emulator
//
// Author:  Tim Stark
// Date:    12/8/2021

#include "emu/core.h"
#include "emu/devsys.h"

#include "terminals/dec/vt100.h"

void vt100_Device::vt100(SystemConfig &config)
{

}

void vt100_Device::vt100_init()
{

}

TERMINAL(vt100, nullptr, dec, vt100, vt100_Device, vt100, vt100_init, "DEC", "VT100 Terminal", SYSTEM_NOT_WORKING)