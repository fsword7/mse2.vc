// dimem.h - Device Interface - Memory
//
// Author:  Tim Stark
// Date:    12/10/2021

#pragma once

#include "emu/map/map.h"
#include "emu/map/addrmap.h"

class diMemory : public DeviceInterface
{
    public:
        diMemory(Device *owner);
        virtual ~diMemory() = default;

        inline Device &getOwningDevice() const { return owner; }

        virtual map::AddressConfigList getAddressConfigList() const = 0;

        inline int getAddressConfigCount() const { return mapAddressConfigList.size(); }
        
        inline map::cAddressConfig *getAddressConfig(map::AddressType space = map::asProgram)
        {
            assert(space < mapAddressConfigList.size());
            return mapAddressConfigList[space];
        }

        void diCompleteConfig() override;

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

        void prepare(UserConsole *user)
        {
            for (auto &space : mapAddressSpaceList)
                if (space != nullptr)
                    space->prepare(user);
        }

        void populate(UserConsole *user)
        { 
            for (auto &space : mapAddressSpaceList)
                if (space != nullptr)
                    space->populate(user);
        }
       
     private:
        Device &owner;

        std::vector<map::cAddressConfig *> mapAddressConfigList;
        std::vector<map::Constructor> mapAddressList;
        std::vector<map::AddressSpace *> mapAddressSpaceList;  
};