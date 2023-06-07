// dimem.h - Device Interface - Memory Interface package
//
// Author:  Tim Stark
// Date:    May 24, 2023

#pragma once

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

    void diCompleteConfig();

    // Address mapping routines

    map::Constructor getAddressMap(map::AddressType space)
    {
        if (space >= 0 && space < mapAddressList.size())
            return mapAddressList[space];
        return map::Constructor();
    }

    template <typename T, typename Return, typename... Args>
    void setAddressMap(map::AddressType space, Return (T::*func)(Args... args))
    {
        Device &cdev = *getOwningDevice().getConfig().getConfigDevice();

        if constexpr(isRelatedClass<Device, T>::value)
        {
            std::cout << fmt::format("{}: ({} - related device) set address list map\n",
                cdev.getsDeviceName(), typeid(T).name());
            setAddressMap(space, map::Constructor(func, cdev.getcDeviceName(), &mse_static_cast<T &>(cdev)));
        }
        else
        {
            std::cout << fmt::format("{}: ({} - unrelated device) set address list map\n",
                cdev.getsDeviceName(), typeid(T).name());         
            setAddressMap(space, map::Constructor(func, cdev.getcDeviceName(), &dynamic_cast<T &>(cdev)));
        }    
    }

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