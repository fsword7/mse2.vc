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

        virtual map::AddressConfigList getAddressConfigList() const = 0;

        void diCompleteConfig() override;

     private:
        std::vector<map::cAddressConfig *> mapAddressConfigList;
        std::vector<map::Constructor> mapAddressList;
        std::vector<map::AddressSpace *> mapAddressSpaceList;  
};