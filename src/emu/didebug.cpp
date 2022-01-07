// didebug.h - Debugging/Disassembler Device Interface package
//
// Author:  Tim Stark
// Date:    Jan 7, 2022

#include "emu/core.h"
#include "emu/didebug.h"

diDebug::diDebug(Device *owner)
: DeviceInterface(owner, "debug")
{ }