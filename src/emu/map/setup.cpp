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
        addrMask = makeBitmask<offs_t>(config.getAddrWidth());
    }

    void AddressSpace::prepare(UserConsole *user)
    {
        fmt::printf("%s: Preparing for %s address space\n",
            device.getDeviceName(), asInfo[space]);

        assert(map == nullptr);
        map = new AddressList(device, space);

        unmapValue = map->unmapValue;
        if (map->gaddrMask != 0ull)
        {
            if (map->gaddrMask & ~addrMask)
                fmt::printf("%s(%s): Can't set a global mask of %0*llX on a %d-bit address width (mask %0*llX)\n",
                    device.getDeviceName(), asInfo[space], config.getAddrPrecision(), map->gaddrMask,
                    config.getAddrPrecision(), addrMask);
            addrMask = map->gaddrMask;
        }

        for (AddressEntry *entry : map->list)
        {
            fmt::printf("%s(%s): Mapping %0*llX-%0*llX mask %0*llX mirror %0*llX\n",
                device.getDeviceName(), asInfo[space],
                config.getAddrPrecision(), entry->addrStart,
                config.getAddrPrecision(), entry->addrEnd,
                config.getAddrPrecision(), entry->addrMask,
                config.getAddrPrecision(), entry->addrMirror);

        }
    }

    void AddressSpace::populate(UserConsole *user)
    {
        fmt::printf("%s: Populating for %s address space\n",
            device.getDeviceName(), asInfo[space]);
    }
}
