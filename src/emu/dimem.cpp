// dimem.cpp - Device Interface - Memory
//
// Author:  Tim Stark
// Date:    12/10/2021

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "emu/dimem.h"

diMemory::diMemory(Device *owner)
: DeviceInterface(owner, "memory")
{ }

void diMemory::diCompleteConfig()
{
    map::AddressConfigList list = getAddressConfigList();

    for (const auto &entry : list)
    {
        if (entry.type >= mapAddressConfigList.size())
            mapAddressConfigList.resize(entry.type + 1);
        mapAddressConfigList[entry.type] = entry.config;
    }
}