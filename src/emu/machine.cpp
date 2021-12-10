// machine.cpp - machine engine package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

#include "emu/core.h"
#include "emu/machine.h"

Machine::Machine(const SystemConfig &config, cstag_t &sysName)
: config(config), sysName(sysName),
  sysDevice(config.getSystemDevice())
{

}

Machine::~Machine()
{
    
}