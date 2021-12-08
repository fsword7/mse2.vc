// addrmap.cpp - mapping address list package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

#include "emu/core.h"
#include "emu/map/addrmap.h"

using namespace aspace;

AddressList::AddressList(Device &dev, int space)
: device(dev), addrSpace(space)
{
    list.clear();
}

AddressList::~AddressList()
{
    for (int idx = 0; idx < list.size(); idx++)
        delete list[idx];
    list.clear();
}

AddressEntry &AddressList::operator () (offs_t start, offs_t end)
{
    AddressEntry *entry = new AddressEntry(device, *this, start, end);
    list.push_back(entry);

    return *entry;
}

// ********

AddressEntry::AddressEntry(Device &dev, AddressList &map, offs_t start, offs_t end)
: device(dev), map(map),
  read8(dev),   read8o(dev),   read8om(dev),
  read16(dev),  read16o(dev),  read16om(dev),
  read32(dev),  read32o(dev),  read32om(dev),
  read64(dev),  read64o(dev),  read64om(dev),
  write8(dev),  write8o(dev),  write8om(dev),
  write16(dev), write16o(dev), write16om(dev),
  write32(dev), write32o(dev), write32om(dev),
  write64(dev), write64o(dev), write64om(dev)
{

}

AddressEntry &AddressEntry::region(ctag_t *name, offs_t off)
{
    // Assign region space to that entry
    regionName = name;
    regionOffset = off;

    return *this;
}

AddressEntry &AddressEntry::mask(offs_t mask)
{
    addrMask = mask;
    // Apply with global address mask
    if (map.gaddrMask != 0)
        addrMask &= ~map.gaddrMask;
    return *this;
}