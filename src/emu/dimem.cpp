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

void diMemory::setAddressMap(map::AddressType space, map::Constructor map)
{
    if (space >= mapAddressList.size())
        mapAddressList.resize(space+1);
    mapAddressList[space] = std::move(map);
}
