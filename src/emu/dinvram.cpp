// dinvram.cpp - Non-Volatile Memory Device Interface package
//
// Author:  Tim Stark
// Date:    Jan 14, 2021

#include "emu/core.h"
#include "emu/dinvram.h"

diNonVolatileMemory::diNonVolatileMemory(Device *owner)
: DeviceInterface(owner, "nvram")
{ }