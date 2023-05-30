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

    virtual map::AddressConfigList getAddressConfigList() const = 0;

    void diCompleteConfig();

    void setAddressMap(map::AddressType space, map::Constructor map);
    

    inline map::cAddressConfig *getAddressConfig(map::AddressType space = map::asProgram)
    {
        if (space >= 0 && space < mapAddressConfigList.size())
            return mapAddressConfigList[space];
        return nullptr;
    }

    // Address space setup routines
    template <typename Space>
    void allocate(map::MemoryManager &manager, map::AddressType space)
    {
        assert((space >= 0) && (space < mapAddressConfigList.size()));
        assert(mapAddressConfigList[space] != nullptr);

        mapAddressSpaceList.resize(std::max(int(mapAddressSpaceList.size()), space+1));
        assert(mapAddressSpaceList[space] == nullptr);

        mapAddressSpaceList[space] = new Space(manager, *this, space, mapAddressConfigList[space]->getAddrWidth());
    }

private:
    Device &owner;

    std::vector<map::cAddressConfig *> mapAddressConfigList;
    std::vector<map::Constructor> mapAddressList;
    std::vector<map::AddressSpace *> mapAddressSpaceList;  
};