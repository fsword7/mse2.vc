// machine.cpp - virtual machine package
//
// Date:    Apr 30, 2023
// Author:  Tim Stark

#include "emu/core.h"
#include "emu/machine.h"

Machine::Machine(cSystemConfig &config, cstr_t &name)
: config(config), sysName(name), 
  sysDevice(config.getSystemDevice()),
  memoryManager(*this)
{
    // Assign this to all devices
    // for (Device &dev : DeviceIterator(*sysDevice))
    //     dev.setMachine(this);
}

Machine *Machine::create(UserConsole *user, cSystemDriver &driver, cstr_t &devName)
{
    SystemConfig *config = new SystemConfig(driver, devName);
    Machine *sysMachine = new Machine(*config, devName);

    return sysMachine;
}

void Machine::setConsole(UserConsole *user)
{

}