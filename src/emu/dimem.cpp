// dimem.cpp - Device Interface - Memory Interface package
//
// Author:  Tim Stark
// Date:    May 24, 2023

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/dimem.h"

diMemory::diMemory(Device *owner)
: DeviceInterface(owner, "memory"), owner(*owner)
{

}

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

void diMemory::setAddressMap(map::AddressType space, map::Constructor map)
{
    if (space >= mapAddressList.size())
        mapAddressList.resize(space+1);
    mapAddressList[space] = std::move(map);
}
