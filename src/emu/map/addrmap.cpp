// addrmap.cpp - mapping address list package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "emu/dimem.h"

using namespace map;

AddressList::AddressList(Device &dev, AddressType space)
: device(dev), addrSpace(space)
{
    list.clear();

    diMemory *bus = nullptr;
    dev.hasInterface(bus);
    assert(bus != nullptr);

    cAddressConfig *config = bus->getAddressConfig(space);
    assert(config != nullptr);

    Constructor map = bus->getAddressMap(space);
    if (!map.isNull())
        map(*this);
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
: device(dev), map(map), addrStart(start), addrEnd(end),
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

// ********

AddressEntry &AddressEntry::r(read8d_t func)
{
    assert(!func.isNull());

    read.type = mapDelegate;
    read.bits = 8;
    read.name = func.getName();
    read8 = func;

    return *this;
}

AddressEntry &AddressEntry::r(read8do_t func)
{
    assert(!func.isNull());

    read.type = mapDelegateO;
    read.bits = 8;
    read.name = func.getName();
    read8o = func;

    return *this;
}

AddressEntry &AddressEntry::r(read8dom_t func)
{
    assert(!func.isNull());

    read.type = mapDelegateOM;
    read.bits = 8;
    read.name = func.getName();
    read8om = func;

    return *this;
}

// ********

AddressEntry &AddressEntry::r(read16d_t func)
{
    assert(!func.isNull());

    read.type = mapDelegate;
    read.bits = 16;
    read.name = func.getName();
    read16 = func;

    return *this;
}

AddressEntry &AddressEntry::r(read16do_t func)
{
    assert(!func.isNull());

    read.type = mapDelegateO;
    read.bits = 16;
    read.name = func.getName();
    read16o = func;

    return *this;
}

AddressEntry &AddressEntry::r(read16dom_t func)
{
    assert(!func.isNull());

    read.type = mapDelegateOM;
    read.bits = 16;
    read.name = func.getName();
    read16om = func;

    return *this;
}

// ********

AddressEntry &AddressEntry::r(read32d_t func)
{
    assert(!func.isNull());

    read.type = mapDelegate;
    read.bits = 32;
    read.name = func.getName();
    read32 = func;

    return *this;
}

AddressEntry &AddressEntry::r(read32do_t func)
{
    assert(!func.isNull());

    read.type = mapDelegateO;
    read.bits = 32;
    read.name = func.getName();
    read32o = func;

    return *this;
}

AddressEntry &AddressEntry::r(read32dom_t func)
{
    assert(!func.isNull());

    read.type = mapDelegateOM;
    read.bits = 32;
    read.name = func.getName();
    read32om = func;

    return *this;
}

// ********

AddressEntry &AddressEntry::r(read64d_t func)
{
    assert(!func.isNull());

    read.type = mapDelegate;
    read.bits = 64;
    read.name = func.getName();
    read64 = func;

    return *this;
}

AddressEntry &AddressEntry::r(read64do_t func)
{
    assert(!func.isNull());

    read.type = mapDelegateO;
    read.bits = 64;
    read.name = func.getName();
    read64o = func;

    return *this;
}

AddressEntry &AddressEntry::r(read64dom_t func)
{
    assert(!func.isNull());

    read.type = mapDelegateOM;
    read.bits = 64;
    read.name = func.getName();
    read64om = func;

    return *this;
}

// ********

AddressEntry &AddressEntry::w(write8d_t func)
{
    assert(!func.isNull());

    write.type = mapDelegate;
    write.bits = 8;
    write.name = func.getName();
    write8 = func;

    return *this;
}

AddressEntry &AddressEntry::w(write8do_t func)
{
    assert(!func.isNull());

    write.type = mapDelegateO;
    write.bits = 8;
    write.name = func.getName();
    write8o = func;

    return *this;
}

AddressEntry &AddressEntry::w(write8dom_t func)
{
    assert(!func.isNull());

    write.type = mapDelegateOM;
    write.bits = 8;
    write.name = func.getName();
    write8om = func;

    return *this;
}

// ********

AddressEntry &AddressEntry::w(write16d_t func)
{
    assert(!func.isNull());

    write.type = mapDelegate;
    write.bits = 16;
    write.name = func.getName();
    write16 = func;

    return *this;
}

AddressEntry &AddressEntry::w(write16do_t func)
{
    assert(!func.isNull());

    write.type = mapDelegateO;
    write.bits = 16;
    write.name = func.getName();
    write16o = func;

    return *this;
}

AddressEntry &AddressEntry::w(write16dom_t func)
{
    assert(!func.isNull());

    write.type = mapDelegateOM;
    write.bits = 16;
    write.name = func.getName();
    write16om = func;

    return *this;
}

// ********

AddressEntry &AddressEntry::w(write32d_t func)
{
    assert(!func.isNull());

    write.type = mapDelegate;
    write.bits = 32;
    write.name = func.getName();
    write32 = func;

    return *this;
}

AddressEntry &AddressEntry::w(write32do_t func)
{
    assert(!func.isNull());

    write.type = mapDelegateO;
    write.bits = 32;
    write.name = func.getName();
    write32o = func;

    return *this;
}

AddressEntry &AddressEntry::w(write32dom_t func)
{
    assert(!func.isNull());

    write.type = mapDelegateOM;
    write.bits = 32;
    write.name = func.getName();
    write32om = func;

    return *this;
}

// ********

AddressEntry &AddressEntry::w(write64d_t func)
{
    assert(!func.isNull());

    write.type = mapDelegate;
    write.bits = 64;
    write.name = func.getName();
    write64 = func;

    return *this;
}

AddressEntry &AddressEntry::w(write64do_t func)
{
    assert(!func.isNull());

    write.type = mapDelegateO;
    write.bits = 64;
    write.name = func.getName();
    write64o = func;

    return *this;
}

AddressEntry &AddressEntry::w(write64dom_t func)
{
    assert(!func.isNull());

    write.type = mapDelegateOM;
    write.bits = 64;
    write.name = func.getName();
    write64om = func;

    return *this;
}