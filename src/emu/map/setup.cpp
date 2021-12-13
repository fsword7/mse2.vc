// setup.cpp - Address space package - setup routines
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/8/2021

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/memmgr.h"
#include "emu/dimem.h"
#include "main/user.h"

namespace map
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

            if (entry->shareName != nullptr)
            {
                MemoryShare *share = manager.findShare(entry->shareName);

                if (share == nullptr)
                {
                    // Allocate new shared memory space
                    size_t bytes = config.convertAddressToByte(entry->addrEnd+1 - entry->addrStart);

                    fmt::printf("%s(%s): creating share '%s' of length %0*llX (%d) bytes\n",
                        device.getDeviceName(), asInfo[space], entry->shareName,
                        config.getAddrPrecision(), bytes, bytes);

                    share = manager.allocateShare(device, space, entry->shareName,
                        bytes, config.getDataWidth(), config.getEndianType());
                    entry->memData = (uint8_t *)share->getData();
                }
                else
                {
                    // Assign existing shared memory space
                    size_t bytes = config.convertAddressToByte(entry->addrEnd+1 - entry->addrStart);

                    if (share->compare(bytes, config.getAddrPrecision(), config.getDataWidth(), config.getEndianType()))
                        entry->memData = (uint8_t *)share->getData();
                    else
                    {
                        fmt::printf("%s(%s): %s\n", device.getDeviceName(), asInfo[space],
                            share->getErrorMessage());
                        // Allocating anonymous memory space safely as default below...
                    }
                }
            }

            if (entry->regionName != nullptr)
            {
                MemoryRegion *region = manager.findRegion(entry->regionName);

                if (region != nullptr)
                {
                    // Determine ending address for expandable memory space
                    if (region->getSize() < (entry->addrEnd - entry->addrStart + 1))
                    {
                        fmt::printf("%s(%s): %0*llX-%0*llX - expandable range up to %0*llX\n",
                            device.getDeviceName(), asInfo[space],
                            config.getAddrPrecision(), entry->addrStart,
                            config.getAddrPrecision(), (entry->addrStart + region->getSize()) - 1,
                            config.getAddrPrecision(), entry->addrEnd,
                            entry->regionName);

                        // Adjust new ending address for desired memory length
                        entry->addrEnd = entry->addrStart + region->getSize() - 1;
                    }

                    // Assign region memory space.
                    entry->memData = region->getData();
                }
                else
                {
                    fmt::printf("%s(%s): %0*llX-%0*llX - non-existant region '%s'\n",
                        device.getDeviceName(), asInfo[space],
                        config.getAddrPrecision(), entry->addrStart,
                        config.getAddrPrecision(), entry->addrEnd,
                        entry->regionName);

                    // Allocating anonymous memory space safely as default below...
                }
            }

            if (entry->memData == nullptr && (entry->read.type == mapROMSpace ||
                entry->read.type == mapRAMSpace || entry->write.type == mapRAMSpace))
            {
                fmt::printf("%s(%s): %0*llX-%0*llX - allocating anonymous memory space\n",
                    device.getDeviceName(), asInfo[space],
                    config.getAddrPrecision(), entry->addrStart,
                    config.getAddrPrecision(), entry->addrEnd);

                entry->memData = manager.allocateMemory(device, space, "(anonymous)",
                    config.convertAddressToByte(entry->addrEnd+1 - entry->addrStart),
                    config.getDataWidth(), config.getEndianType());
            }
        }
    }

    void AddressSpace::populate(UserConsole *user)
    {
        fmt::printf("%s: Populating for %s address space\n",
            device.getDeviceName(), asInfo[space]);
    }
}
