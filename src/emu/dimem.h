// dimem.h - Device Interface - Memory Interface package
//
// Author:  Tim Stark
// Date:    May 24, 2023

#pragma once

class diMemory : public DeviceInterface
{
public:
    diMemory(Device *owner);
    virtual ~diMemory() = default;

    inline Device &getOwningDevice() const { return owner; }

    virtual map::AddressConfigList getAddressConfigList() = 0;

    void setAddressMap(map::AddressType space, map::Constructor map);

private:
    Device &owner;

    std::vector<map::cAddressConfig *> mapAddressConfigList;
    std::vector<map::Constructor> mapAddressList;
    std::vector<map::AddressSpace *> mapAddressSpaceList;  
};