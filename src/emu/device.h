// device.h - device management package
//
// Date:    Apr 30, 2023
// Author:  Tim Stark

#pragma once

#include <type_traits>

#define DEVICE_SELF ""

// System creator package
template<class SystemClass, cchar_t *shortName, cchar_t *fullName, cchar_t *fileName>
struct systemNameStruct
{
    typedef SystemClass type;
};

template<class SystemClass, cchar_t *shortName, cchar_t *fullName, cchar_t *fileName>
auto systemNameFunc()
{
    return systemNameStruct<SystemClass, shortName, fullName, fileName>{};
}

template<class SystemClass, cchar_t *shortName, cchar_t *fullName, cchar_t *fileName>
constexpr auto systemCreator = &systemNameFunc<SystemClass, shortName, fullName, fileName>;


// Device creator package
template<class DeviceClass, cchar_t *shortName, cchar_t *fullName, cchar_t *fileName>
struct deviceNameStruct
{
    typedef DeviceClass type;
};

template<class DeviceClass, cchar_t *shortName, cchar_t *fullName, cchar_t *fileName>
auto deviceNameFunc()
{
    return deviceNameStruct<DeviceClass, shortName, fullName, fileName>{};
}

template<class DeviceClass, cchar_t *shortName, cchar_t *fullName, cchar_t *fileName>
constexpr auto deviceCreator = &systemNameFunc<DeviceClass, shortName, fullName, fileName>;



class DeviceType
{
private:
    typedef Device *(*createFunc)(const SystemConfig &config, const DeviceType &type,
        cstr_t &name, Device *owner, uint64_t clock);

    template<typename SystemClass>
    static Device *createSystem(const SystemConfig &config, const DeviceType &type,
        cstr_t &name, Device *owner, uint64_t clock)
    {
        return new SystemClass(config, type, name, clock);
    }

    template<typename DeviceClass>
    static Device *createDevice(const SystemConfig &config, const DeviceType &type,
        cstr_t &name, Device *owner, uint64_t clock)
    {
        return new DeviceClass(config, name, owner, clock);
    }

public:
    DeviceType() = default;

    template<class SystemClass, cchar_t *shortName, cchar_t *fullName, cchar_t *fileName>
    DeviceType(systemNameStruct<SystemClass, shortName, fullName, fileName>(*)())
    : idType(typeid(SystemClass)), creator(&createSystem<SystemClass>),
      shortName(shortName), fullName(fullName), fileName(fileName)
    { }

    template<class DeviceClass, cchar_t *shortName, cchar_t *fullName, cchar_t *fileName>
    DeviceType(deviceNameStruct<DeviceClass, shortName, fullName, fileName>(*)())
    : idType(typeid(DeviceClass)), creator(&createDevice<DeviceClass>),
      shortName(shortName), fullName(fullName), fileName(fileName)
    { }

    Device *create(const SystemConfig &config, cstr_t &name, Device *owner, uint64_t clock) const
    {
        assert(creator != nullptr);
        return creator(config, *this, name, owner, clock);
    }

    // Getter function calls
    inline const std::type_info &getType() const    { return idType; }
    inline cchar_t *getFullName() const             { return fullName; }
    inline cchar_t *getShortName() const            { return shortName; }
    inline cchar_t *getSourceName() const           { return fileName; }

private:
    const std::type_info &idType = typeid(std::nullptr_t);
    createFunc creator = nullptr;

    cchar_t *shortName = nullptr;
    cchar_t *fullName = nullptr;
    cchar_t *fileName = nullptr;
};
template <class DeviceClass>
class DeviceCreator : public DeviceType
{
public:
    using DeviceType::DeviceType;
    using DeviceType::create;

    template <typename... Args>
    DeviceClass *create(SystemConfig &config, cstr_t &devName, Device *owner, Args &&... args) const
    {
        return new DeviceClass(config, devName, owner, std::forward<Args>(args)...);
    }

    // For complete function calls, see them in templates.h file.
    
    template <typename... Args>
    DeviceClass *operator () (SystemConfig &config, cstr_t &devName, Args&&... args) const;

    // template <typename Exposed, bool Required, typename... Args>
    // DeviceClass &operator () (SystemConfig &config, DeviceFinder<Exposed, Required> &finder,
    //     cstr_t &devName, Args &&... args) const;

};

#define DECLARE_DEVICE_TYPE(Type, Class) \
extern DeviceCreator<Class> const &Type;

// #define DECLARE_DEVICE_TYPE(Type, Class)          \
// class Class;                                      \
// extern template class DeviceFinder<Class, false>; \
// extern template class DeviceFinder<Class, true>;

// #define DECLARE_DEVICE_TYPE_NS(Type, Namespace, Class)       \
// namespace Namespace { class Class; }                         \
// extern template class DeviceFinder<Namespace::Class, false>; \
// extern template class DeviceFinder<Namespace::Class, true>;

#define DEFINE_DEVICE_TYPE(Type, Class, ShortName, FullName) \
struct Class##_device_traits {                               \
    static constexpr cchar_t shortName[] = ShortName;        \
    static constexpr cchar_t fullName[]  = FullName;         \
    static constexpr cchar_t fileName[]  = __FILE__;         \
};                                                           \
constexpr cchar_t Class##_device_traits::shortName[];        \
constexpr cchar_t Class##_device_traits::fullName[];         \
constexpr cchar_t Class##_device_traits::fileName[];         \
DeviceCreator<Class> const &Type = deviceCreator<Class,      \
    (Class##_device_traits::shortName),                      \
    (Class##_device_traits::fullName),                       \
    (Class##_device_traits::fileName)>;

using cDeviceType = const DeviceType;

class Device : public BindedObject, public List<Device>
{
public:
    using ifaceList_t = std::vector<DeviceInterface *>;

    virtual ~Device() = default;

    Device *getOwner() const            { return owner; }
    str_t getsDeviceName() const        { return devName; }
    cchar_t *getcDeviceName() const     { return devName.c_str(); }
    cchar_t *getFullName() const        { return type.getFullName(); }
    cchar_t *getShortName() const       { return type.getShortName(); }
    cchar_t *getSourceName() const      { return type.getSourceName(); }

    void configure(SystemConfig &config);
    void start();
    void stop();
    void reset();

    void addInterface(DeviceInterface *iface);
    void finishConfig();

    // Mapping calls
    void resolveFinalMapping();
    bool findObjects();

    // Virtual device function calls
    void devConfigure(SystemConfig &config) {}
    void devStart() {}
    void devStop() {}
    void devReset() {}

	// Dynamic_cast safely converts references and pointers to up, down and sideways. 
	// If cast fails, return null pointer. For more information, check
	// https://en.cppreference.com/w/cpp/language/dynamic_cast.

    template <class DeviceClass>
    inline bool hasInterface(DeviceClass *&iface)
    {
        return (iface = dynamic_cast<DeviceClass *>(this)) != nullptr;
    }

    template <class DeviceClass>
    inline bool hasInterface(DeviceClass *&iface) const
    {
        return (iface = dynamic_cast<const DeviceClass *>(this)) != nullptr;
    }

protected:
    Device(const SystemConfig &config, cDeviceType &type, cstr_t &name, Device *owner, uint64_t clock);

private:
    cDeviceType &type;
    cSystemConfig &sysConfig;
    bool startedFlag = false;

    Device *owner = nullptr;
    uint64_t clock = 0;

    cstr_t devName;

    ifaceList_t ifaceList;
};
class DeviceInterface
{
    friend class Device;

public:
    DeviceInterface(Device *owner, cstr_t &name);
    virtual ~DeviceInterface() = default;

    inline Device *getOwningDevice() const  { return owner; }
    inline cstr_t  getsName() const         { return diName; }
    inline cchar_t *getcName() const        { return diName.c_str(); }

    // Virtual device interface function calls
    virtual void diCompleteConfig() {}
    virtual void diUpdateClock() {}

private:
    Device *owner;
    cstr_t diName;
};

// ******
class DeviceIterator
{
public:
    DeviceIterator(Device &dev) : devRoot(dev) {}

    class Iterator
    {
    public:
        Iterator(Device *device) : curDevice(device) {}

        // Required operator function calls
        // bool operator == (const Iterator &iter) { return curDevice == iter.curDevice; }
        bool operator != (const Iterator &iter) { return curDevice != iter.curDevice; }
        Iterator operator ++ () { advance(); return *this; }
        Iterator operator ++ (int) { const Iterator result(*this); ++*this; return result; }
        Device &operator * () { assert(curDevice != nullptr); return *curDevice; }
        Device *operator -> () { return curDevice; }

    protected:
        void advance()
        {
            if (curDevice == nullptr)
                return;
            
            if (curDevice->hasChildren())
            {
                ownDevice = curDevice;
                curDevice = ownDevice->getFirst();
                depth++;

                return;
            }

            while (depth > 0 && ownDevice != nullptr)
            {
                curDevice = ownDevice->getNext();
                if (curDevice != nullptr)
                    return;

                curDevice = ownDevice;
                ownDevice = ownDevice->getOwner();
                depth--;
            }
            curDevice = nullptr;
        }

        Device *curDevice = nullptr;
        Device *ownDevice = nullptr;
        int depth = 0;
    };

    Iterator begin() { return Iterator(&devRoot); }
    Iterator end()   { return Iterator(nullptr); }

private:
    Device &devRoot;
};

template <class InterfaceType>
class InterfaceIterator
{
public:
    InterfaceIterator(Device &dev) : devRoot(dev) {}

    class Iterator : public DeviceIterator::Iterator
    {
    public:
        Iterator(Device *device) : DeviceIterator::Iterator(device) { findInterface(); }

        // Required operator function calls
        // bool operator == (const Iterator &iter) { return iface == iter.iface; }
        bool operator != (const Iterator &iter) { return iface != iter.iface; }
        Iterator operator ++ () { advance(); findInterface(); return *this; }
        Iterator operator ++ (int) { const Iterator result(*this); ++*this; return result; }
        InterfaceType &operator * () { assert(iface != nullptr); return *iface; }
        InterfaceType *operator -> () { return iface; }

    private:
        void findInterface()
        {
            for (; curDevice != nullptr; advance())
            {
                if (curDevice->hasInterface(iface))
                    return;
            }
            iface = nullptr;
        }

        InterfaceType *iface = nullptr;
    };

    Iterator begin() { return Iterator(&devRoot); }
    Iterator end()   { return Iterator(nullptr); }

private:
    Device &devRoot;
};
