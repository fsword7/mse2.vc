// machine.cpp - virtual machine package
//
// Date:    Apr 30, 2023
// Author:  Tim Stark

#include "emu/core.h"
#include "emu/machine.h"

Machine::Machine(UserConsole *user, cSystemDriver &driver, cstr_t &name)
: sysName(name), user(user),
  memoryManager(*this),
  config(driver, name, *this)
{
    sysDevice = config.getSystemDevice();
}

void Machine::setConsole(UserConsole *user)
{
}

void Machine::startAllDevices(UserConsole *user)
{
    for (Device &dev : DeviceIterator(*sysDevice))
    {
      std::cout << fmt::format("{}: starting {} device...\n",
          dev.getcDeviceName(), dev.getShortName());
      dev.start();
    }
}

// starting machine during post-configuration stage
void Machine::start(UserConsole *user)
{
    assert(sysDevice != nullptr);

    memoryManager.init(user);

    // Finding required objects to being linked
    for (Device &dev : DeviceIterator(*sysDevice))
        dev.resolveFinalMapping();

    startAllDevices(user);
}