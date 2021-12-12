// machine.cpp - machine engine package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

#include "emu/core.h"
#include "emu/machine.h"
#include "main/user.h"

Machine::Machine(const SystemConfig &config, cstag_t &sysName)
: memoryManager(*this), config(config), sysName(sysName),
  sysDevice(config.getSystemDevice())
{

}

Machine::~Machine()
{
    
}

Machine *Machine::create(UserConsole *user, const SystemDriver *driver, cstag_t &devName)
{
    SystemConfig *config = new SystemConfig(*driver, devName);
    Machine *sysMachine = new Machine(*config, devName);

    return sysMachine;
}

