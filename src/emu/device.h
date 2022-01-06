// device.h - device/interface package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    Dec 6, 2021

#include <type_traits>

#include "lib/util/list.h"

#define DEVICE_SELF "" // std::string()

class Machine;
class Device;
class DeviceInterface;
class ObjectFinder;

template <typename Exposed, bool Required> class DeviceFinder;

namespace map {
    class MemoryRegion;
    class MemoryBank;
    class MemoryShare;
}

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
        assert(creator != nullptr);
        return creator(config, *this, name, owner, clock);
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

    // For complete function calls, see them in templates.h file.
    
    template <typename... Args>
    DeviceClass *operator () (SystemConfig &config, cstag_t &devName, Args&&... args) const;

    template <typename Exposed, bool Required, typename... Args>
    DeviceClass &operator () (SystemConfig &config, DeviceFinder<Exposed, Required> &finder,
        cstag_t &devName, Args &&... args) const;

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
    static constexpr ctag_t shortName[] = ShortName;         \
    static constexpr ctag_t fullName[]  = FullName;          \
    static constexpr ctag_t fileName[]  = __FILE__;          \
};                                                           \
constexpr ctag_t Class##_device_traits::shortName[];         \
constexpr ctag_t Class##_device_traits::fullName[];          \
constexpr ctag_t Class##_device_traits::fileName[];          \
DeviceCreator<Class> const &Type = deviceCreator<Class,      \
    (Class##_device_traits::shortName),                      \
    (Class##_device_traits::fullName),                       \
    (Class##_device_traits::fileName)>;

// ********

class Device : public BindedObject, public List<Device>
{
public:
    using ifaceList_t = std::vector<DeviceInterface *>;

    virtual ~Device() = default;

    // Getter function calls
    inline const SystemConfig &getConfig() const    { return sysConfig; }
    inline Device *getOwner() const                 { return owner; }
    inline uint64_t getClock() const                { return clock; }
    inline cstag_t &getDeviceName() const           { return devName; } // old function call - to be removed later
    inline std::string getsDeviceName() const       { return devName; }
    inline cchar_t *getcDeviceName() const          { return devName.c_str(); }
    inline std::string getsPathName() const         { return pathName; }
    inline cchar_t *getcPathName() const            { return pathName.c_str(); }
    inline ctag_t *getFullName() const  { return type.getFullName(); }
    inline ctag_t *getShortName() const { return type.getShortName(); }

    inline const SystemConfig &getSystemConfig() const { return sysConfig; }

    inline void setMachine(Machine *owner) { ownMachine = owner; }
    
    void configure(SystemConfig &config);
    void addInterface(DeviceInterface *iface);
    void finishConfig();
    void resolvePostMapping();

    Device *findDevice(ctag_t *name);
    cfwEntry_t *getFirmwareEntries();

    std::string getFullDeviceName(cstag_t tagName); // old function call
    std::string expandPathName(cstag_t &pathName) const;

    map::MemoryRegion *findMemoryRegion(cstag_t &name) const;
    map::MemoryBank *findMemoryBank(cstag_t &name) const;
    map::MemoryShare *findMemoryShare(cstag_t &name) const;

    void registerObject(ObjectFinder *object);
    bool findObjects();

    // Virtual device function calls
    virtual void devConfigure(SystemConfig &config) {}
    virtual cfwEntry_t *devGetFirmwareEntries() { return nullptr; }
    virtual void devResolveObjects() {}
    virtual void devStart() {}

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
    Device(const SystemConfig &config, const DeviceType &type, cstag_t &name, Device *owner, uint64_t clock);

private:
    const DeviceType   &type;
    const SystemConfig &sysConfig;

    ifaceList_t ifaceList;
    std::vector<ObjectFinder *> objectList;

    Machine *ownMachine = nullptr;
    Device  *owner = nullptr;

    uint64_t clock = 0;

    std::string devName;  // Base of device name
    std::string pathName; // full path device name

    cfwEntry_t *fwEntries = nullptr;

protected:


};

class DeviceInterface
{
    friend class Device;

public:
    DeviceInterface(Device *owner, ctag_t *name);
    virtual ~DeviceInterface() = default;

    inline Device *getOwningDevice() const { return owner; }
    inline ctag_t *getName() const         { return diName; }

    // Virtual device interface function calls
    virtual void diCompleteConfig() {}

private:
    Device *owner;
    ctag_t *diName;
};

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
