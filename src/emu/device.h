// device.h - device/interface package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    Dec 6, 2021

#include <type_traits>

#include "lib/util/list.h"

class Device;
class DeviceInterface;

template <typename T> struct isDevice
{
    static constexpr bool value = std::is_base_of<Device, T>::value;
};

template <typename T> struct isInterface
{
    static constexpr bool value = std::is_base_of<DeviceInterface, T>::value;
};

// System creator package
template<class SystemClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
struct systemTagStruct
{
    typedef SystemClass type;
};

template <class SystemClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
auto systemTagFunc()
{
    return systemTagStruct<SystemClass, shortName, fullName, fileName>{};
}

template <class SystemClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
constexpr auto systemCreator = &systemTagFunc<SystemClass, shortName, fullName, fileName>;

// Device creator package
template<class DeviceClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
struct deviceTagStruct
{
    typedef DeviceClass type;
};

template <class DeviceClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
auto deviceTagFunc()
{
    return deviceTagStruct<DeviceClass, shortName, fullName, fileName>{};
}

template <class DeviceClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
constexpr auto deviceCreator = &deviceTagFunc<DeviceClass, shortName, fullName, fileName>;


class DeviceType
{
private:
    typedef Device *(*createFunc)(const SystemConfig &config, const DeviceType &type,
        cstag_t &tagName, Device *owner, uint64_t clock);
    
    template <typename SystemClass>
    static Device *createSystem(const SystemConfig &config, const DeviceType &type,
        cstag_t &tagName, Device *owner, uint64_t clock)
    {
        return new SystemClass(config, type, tagName, clock);
    }

    template <typename DeviceClass>
    static Device *createDevice(const SystemConfig &config, const DeviceType &type,
        cstag_t &tagName, Device *owner, uint64_t clock)
    {
        return new DeviceClass(config, tagName, owner, clock);
    }
   
public:
    DeviceType() = default;

    template <class SystemClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
    DeviceType(systemTagStruct<SystemClass, shortName, fullName, fileName>(*)())
    : idType(typeid(SystemClass)), creator(&createSystem<SystemClass>),
      shortName(shortName), fullName(fullName), fileName(fileName)
    { }

    template <class DeviceClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
    DeviceType(deviceTagStruct<DeviceClass, shortName, fullName, fileName>(*)())
    : idType(typeid(DeviceClass)), creator(&createDevice<DeviceClass>),
      shortName(shortName), fullName(fullName), fileName(fileName)
    { }

    Device *create(const SystemConfig &config, cstag_t &name, Device *owner, uint64_t clock) const
    {
        return nullptr;
    }

    // Getter function calls
    inline const std::type_info &getType() const    { return idType; }
    inline ctag_t *getFullName() const              { return fullName; }
    inline ctag_t *getShortName() const             { return shortName; }
    inline ctag_t *getSourceName() const            { return fileName; }

private:
    const std::type_info &idType = typeid(std::nullptr_t); // device type identification
    createFunc creator = nullptr;

    ctag_t  *shortName = nullptr;   // Short name
    ctag_t  *fullName = nullptr;    // Full name/description
    ctag_t  *fileName = nullptr;    // Name of source file
};

template <class DeviceClass>
class DeviceCreator : public DeviceType
{
public:
    using DeviceType::DeviceType;
    using DeviceType::create;

    template <typename... Args>
    DeviceClass *create(SystemConfig &config, cstag_t &devName, Device *owner, Args &&... args) const
    {
        return new DeviceClass(config, devName, owner, std::forward<Args>(args)...);
    }

    template <typename... Args>
    DeviceClass *operator () (SystemConfig &config, cstag_t &devName, Args&&... args) const;

    // template <typename... Exposed, bool Required, typename... Args>
    // DeviceClass *operator () (SystemConfig &config, DeviceFinder<Exposed, Required> &finder, cstag_t &devName, Args &&... args) const;

};

#define DECLARE_DEVICE_TYPE(Type, Class) \
extern DeviceCreator<Class> const &type;

// ********

class Device : public BindedObject, public List<Device>
{
public:
    virtual ~Device() = default;

    // Getter function calls
    inline const SystemConfig &getConfig() const    { return config; }
    inline Device *getOwner() const                 { return owner; }
    inline uint64_t getClock() const                { return clock; }
    inline cstag_t &getDeviceName() const           { return devName; }
    inline cstag_t &getTagName() const              { return tagName; }

    void configure(SystemConfig &config);

protected:
    Device(const SystemConfig &config, const DeviceType &type, cstag_t &name, Device *owner, uint64_t clock);

private:
    const DeviceType &type;
    const SystemConfig &config;

    Device *owner = nullptr;

    uint64_t clock = 0;
    cstag_t  devName;
    cstag_t  tagName;

protected:


};

class DeviceInterface
{
public:
    DeviceInterface() = default;
    virtual ~DeviceInterface() = default;

private:

};