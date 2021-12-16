// dimem.h - Device Interface - Memory
//
// Author:  Tim Stark
// Date:    12/10/2021

#pragma once

#include "emu/map/map.h"
#include "emu/map/addrmap.h"

class diMemory : public DeviceInterface
{
    private:
        template <typename T, typename U>
        struct isRelatedClass {
            static constexpr bool value = std::is_convertible<std::add_pointer<T>, std::add_pointer<U>>::value;
        };

        template <typename T, typename U>
        struct isRelatedDevice {
            static constexpr bool value = isDevice<T>::value && isRelatedClass<T, U>::value;
        };

        template <typename T, typename U>
        struct isRelatedInterface {
            static constexpr bool value = isInterface<T>::value && isRelatedClass<T, U>::value;
        };

        template <typename T, typename U>
        struct isUnrelatedDevice {
            static constexpr bool value = isDevice<T>::value && !isRelatedClass<T, U>::value;
        };

        template <typename T, typename U>
        struct isUnrelatedInterface {
            static constexpr bool value = isInterface<T>::value && !isRelatedClass<T, U>::value;
        };

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

        // template <typename T, typename U, typename Return, typename... Args>
        // std::enable_if_t<isUnrelatedDevice<Device, T>::value>
        // setAddressMap(map::AddressType space, T &obj, Return (U::*func)(Args...))
        // {
        //     setAddressMap(space, map::Constructor(func, obj.getDeviceName(), &dynamic_cast<U &>(obj)));
        // }

        template <typename T, typename Return, typename... Args>
        std::enable_if_t<isRelatedDevice<Device, T>::value>
        setAddressMap(map::AddressType space, Return (T::*func)(Args...))
        {
            const SystemConfig &config = getOwningDevice().getConfig();
            Device *dev = config.getConfigDevice();

            fmt::printf("%s: (related device) set address list map\n", dev->getDeviceName());
            setAddressMap(space, map::Constructor(func, dev->getDeviceName().c_str(), &dynamic_cast<T &>(*dev)));
        }

        template <typename T, typename Return, typename... Args>
        std::enable_if_t<isUnrelatedDevice<Device, T>::value>
        setAddressMap(map::AddressType space, Return (T::*func)(Args...))
        {
            const SystemConfig &config = getOwningDevice().getConfig();
            Device *dev = config.getConfigDevice();

            fmt::printf("%s: (unrelated device) set address list map\n", dev->getDeviceName());
            setAddressMap(space, map::Constructor(func, dev->getDeviceName().c_str(), &dynamic_cast<T &>(*dev)));
        }

        void setAddressMap(map::AddressType space, map::Constructor map);

        map::Constructor getAddressMap(map::AddressType space)
        {
            if (space >= 0 && space < mapAddressList.size())
                return mapAddressList[space];
            return map::Constructor();
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