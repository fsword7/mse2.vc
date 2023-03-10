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
    ctag_t *asInfo[] = { "program", "data", "io" };

    AddressSpace::AddressSpace(MemoryManager &manager, diMemory &bus, AddressType space)
    : AddressSpaceInstaller(), space(space), config(*bus.getAddressConfig(space)),
      device(bus.getOwningDevice()), manager(manager)
    {
        addrMask = makeBitmask<offs_t>(config.getAddrWidth());
    }

    void AddressSpace::prepare(UserConsole *user)
    {
        assert(map == nullptr);
        map = new AddressList(device, space);

        fmt::printf("%s: Preparing for %s address space\n",
            device.getsDeviceName(), asInfo[space]);

        unmapValue = map->unmapValue;
        if (map->gaddrMask != 0ull)
        {
            if (map->gaddrMask & ~addrMask)
                fmt::printf("%s.%s: Can't set a global mask of %0*llX on a %d-bit address width (mask %0*llX)\n",
                    device.getsDeviceName(), asInfo[space], config.getAddrPrecision(), map->gaddrMask,
                    config.getAddrPrecision(), addrMask);
            addrMask = map->gaddrMask;
        }

        for (AddressEntry *entry : map->list)
        {
            fmt::printf("%s.%s: Mapping %0*llX-%0*llX mask %0*llX mirror %0*llX\n",
                entry->device.getsDeviceName(), asInfo[space],
                config.getAddrPrecision(), entry->addrStart,
                config.getAddrPrecision(), entry->addrEnd,
                config.getAddrPrecision(), entry->addrMask,
                config.getAddrPrecision(), entry->addrMirror);

            if (entry->shareName != nullptr)
            {
                cstag_t fullName = entry->device.expandPathName(entry->shareName);
                MemoryShare *share = manager.findShare(fullName);

                if (share == nullptr)
                {
                    // Allocate new shared memory space
                    size_t bytes = config.convertAddressToByte(entry->addrEnd+1 - entry->addrStart);

                    fmt::printf("%s.%s: creating share '%s' of length %0*llX (%d) bytes\n",
                        entry->device.getsDeviceName(), asInfo[space],
                        fullName, config.getAddrPrecision(), bytes, bytes);

                    share = manager.allocateShare(entry->device, space, fullName,
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
                        fmt::printf("%s.%s: %s\n", entry->device.getsDeviceName(),
                            asInfo[space], share->getErrorMessage());
                        // Allocating anonymous memory space safely as default below...
                    }
                }
            }

            // If this is ROM handler without a specified region name and not shared,
            // must attach it to the implicit region under device name.
            if (space == map::asProgram && entry->read.type == mapROMSpace &&
                entry->regionName == nullptr && entry->shareName == nullptr)
            {
                MemoryRegion *region = device.findMemoryRegion(DEVICE_SELF);

                if (region != nullptr && entry->addrEnd < region->getBytes())
                {
                    entry->regionName = device.getcDeviceName();
                    // entry->regionName = device.getcPathName();
                    entry->regionOffset = config.convertAddressToByte(entry->addrStart);
                }
                else if (region == nullptr)
                {
                    fmt::printf("%s.%s: %0*llX-%0*llX - not attached with required region '%s' space\n",
                        entry->device.getsDeviceName(), asInfo[space],
                        config.getAddrPrecision(), entry->addrStart,
                        config.getAddrPrecision(), entry->addrEnd,
                        device.getsPathName());
                }
                else
                {
                    fmt::printf("%s.%s: %0*llX-%0*llX - region '%s' out of range\n",
                        entry->device.getsDeviceName(), asInfo[space],
                        config.getAddrPrecision(), entry->addrStart,
                        config.getAddrPrecision(), entry->addrEnd,
                        device.getsPathName());
                }
            }

            if (entry->regionName != nullptr)
            {
                cstag_t fullName = entry->device.expandPathName(entry->regionName);
                MemoryRegion *region = manager.findRegion(fullName);

                if (region != nullptr)
                {
                    fmt::printf("%s.%s: %0*llX-%0*llX - attached region '%s' space\n",
                        entry->device.getsDeviceName(), asInfo[space],
                        config.getAddrPrecision(), entry->addrStart,
                        config.getAddrPrecision(), entry->addrEnd,
                        fullName);

                    // Determine ending address for expandable memory space
                    if (region->getSize() < (entry->addrEnd - entry->addrStart + 1))
                    {
                        fmt::printf("%s.%s: %0*llX-%0*llX - expandable range up to %0*llX\n",
                            entry->device.getsDeviceName(), asInfo[space],
                            config.getAddrPrecision(), entry->addrStart,
                            config.getAddrPrecision(), (entry->addrStart + region->getSize()) - 1,
                            config.getAddrPrecision(), entry->addrEnd);

                        // Adjust new ending address for desired memory length
                        entry->addrEnd = entry->addrStart + region->getSize() - 1;
                    }

                    // Assign region memory space.
                    entry->memData = region->getData() + entry->regionOffset;
                }
                else
                {
                    fmt::printf("%s.%s: %0*llX-%0*llX - non-existant region '%s'\n",
                        entry->device.getsDeviceName(), asInfo[space],
                        config.getAddrPrecision(), entry->addrStart,
                        config.getAddrPrecision(), entry->addrEnd,
                        fullName);

                    // Allocating anonymous memory space safely as default below...
                }
            }

            if (entry->memData == nullptr && (entry->read.type == mapROMSpace ||
                entry->read.type == mapRAMSpace || entry->write.type == mapRAMSpace))
            {
                fmt::printf("%s.%s: %0*llX-%0*llX - allocating anonymous memory space\n",
                    entry->device.getsDeviceName(), asInfo[space],
                    config.getAddrPrecision(), entry->addrStart,
                    config.getAddrPrecision(), entry->addrEnd);

                entry->memData = manager.allocateMemory(entry->device, space, "(anonymous)",
                    config.convertAddressToByte(entry->addrEnd+1 - entry->addrStart),
                    config.getDataWidth(), config.getEndianType());
            }
        }
    }

    void AddressSpace::populateEntry(const AddressEntry *entry, AccessType acc)
    {
        mapHandler data = (acc == accRead) ? entry->read : entry->write;
        MemoryBank *bank = nullptr;

        switch (data.type)
        {
            case mapNone:
                return;

            case mapNOP:
            case mapUnmapped:
                setUnmapped(entry->addrStart, entry->addrEnd, entry->addrMirror, 0, acc, data.type == mapNOP);
                break;

            case mapROMSpace:
                if (acc == accWrite)
                    return;
                // Do nothing
            case mapRAMSpace:
                setMemorySpace(entry->addrStart, entry->addrEnd, entry->addrMirror, entry->memData, acc);
                break;
            
            case mapBank:
                setMemoryBank(entry->addrStart, entry->addrEnd, entry->addrMirror, bank, 0, acc);
                break;

            case mapView:
                if (acc == accRead)
                    setMemoryView(entry->addrStart, entry->addrEnd, entry->addrMirror, entry->mview);
                break;

            case mapDelegate:
                if (acc == accRead)
                {
                    switch (data.bits)
                    {
                        case 8:
                            setReadHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->read8);
                            break;
                        case 16:
                            setReadHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->read16);
                            break;
                        case 32:
                            setReadHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->read32);
                            break;
                        case 64:
                            setReadHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->read64);
                            break;
                    }
                }
                else if (acc == accWrite)
                {
                    switch (data.bits)
                    {
                        case 8:
                            setWriteHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->write8);
                            break;
                        case 16:
                            setWriteHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->write16);
                            break;
                        case 32:
                            setWriteHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->write32);
                            break;
                        case 64:
                            setWriteHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->write64);
                            break;
                    }
                }
                break;

            case mapDelegateO:
                if (acc == accRead)
                {
                    switch (data.bits)
                    {
                        case 8:
                            setReadHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->read8o);
                            break;
                        case 16:
                            setReadHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->read16o);
                            break;
                        case 32:
                            setReadHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->read32o);
                            break;
                        case 64:
                            setReadHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->read64o);
                            break;
                    }
                }
                else if (acc == accWrite)
                {
                    switch (data.bits)
                    {
                        case 8:
                            setWriteHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->write8o);
                            break;
                        case 16:
                            setWriteHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->write16o);
                            break;
                        case 32:
                            setWriteHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->write32o);
                            break;
                        case 64:
                            setWriteHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->write64o);
                            break;
                    }                    
                }
                break;

            case mapDelegateOM:
                if (acc == accRead)
                {
                    switch (data.bits)
                    {
                        case 8:
                            setReadHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->read8om);
                            break;
                        case 16:
                            setReadHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->read16om);
                            break;
                        case 32:
                            setReadHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->read32om);
                            break;
                        case 64:
                            setReadHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->read64om);
                            break;
                    }
                }
                else if (acc == accWrite)
                {
                    switch (data.bits)
                    {
                        case 8:
                            setWriteHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->write8om);
                            break;
                        case 16:
                            setWriteHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->write16om);
                            break;
                        case 32:
                            setWriteHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->write32om);
                            break;
                        case 64:
                            setWriteHandler(entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror,
                                entry->write64om);
                            break;
                    }                   
                }
                break;

            default:
                break;
        }
    }

    void AddressSpace::populate(UserConsole *user)
    {
        fmt::printf("%s: Populating for %s address space\n",
            device.getsDeviceName(), asInfo[space]);

        assert(map != nullptr);

        for (AddressEntry *entry : map->list)
        {
            populateEntry(entry, accRead);
            populateEntry(entry, accWrite);
        }
    }
}
