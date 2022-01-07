// diexec.cpp - Execution Device Interface package
//
// Author:  Tim Stark
// Date:    Dec 10, 2021

#include "emu/core.h"
#include "emu/diexec.h"

diExecute::diExecute(Device *owner)
: DeviceInterface(owner, "execute")
{ }