// addrmap.cpp - Address Mapping package
//
// Author:  Tim Stark
// Date:    May 24, 2023

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "emu/dimem.h"

using namespace map;

AddressList::AddressList(Device &dev, AddressType space)
: device(dev), addrSpace(space)
{
    // Initializa address list
    list.clear();

    // Obtain memory interface from device
    diMemory *bus = nullptr;
    dev.hasInterface(bus);
    assert(bus != nullptr);



}

AddressList::~AddressList()
{
    for (int idx = 0; idx < list.size(); idx++)
        delete list[idx];
    list.clear();
}

AddressEntry &AddressList::operator() (offs_t start, offs_t end)
{
    AddressEntry *entry = new AddressEntry(*owner, *this, start, end);
    list.push_back(entry);

    return *entry;
}

// ******

AddressEntry::AddressEntry(Device &dev, AddressList &map, offs_t start, offs_t end)
: device(dev), map(map), addrStart(start), addrEnd(end)
{

}
