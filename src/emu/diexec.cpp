// diexec.cpp - Device Interface - Execution
//
// Author:  Tim Stark
// Date:    12/10/2021

#include "emu/core.h"
#include "emu/diexec.h"

diExecute::diExecute(Device *owner)
: DeviceInterface(owner, "execute")
{ }