// setup.cpp - Address space package - setup routines
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/8/2021

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/memmgr.h"
#include "emu/dimem.h"
#include "main/user.h"

namespace aspace
{
    static ctag_t *asInfo[] = { "program", "data", "I/O port" };

    AddressSpace::AddressSpace(MemoryManager &manager, diMemory &bus, AddressType space)
    : AddressSpaceInstaller(), space(space), config(*bus.getAddressConfig(space)),
      device(bus.getOwningDevice()), manager(manager)
    {

    }

    void AddressSpace::prepare(UserConsole *user)
    {
        fmt::printf("%s: Preparing for %s address space\n",
            device.getDeviceName(), asInfo[space]);
    }

    void AddressSpace::populate(UserConsole *user)
    {

    }
}
