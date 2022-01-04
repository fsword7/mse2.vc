// device.cpp - Device/Interface package
//
// Author:  Tim Stark
// Date:    Dec 6, 2021

#include "emu/core.h"
#include "emu/machine.h"

Device::Device(const SystemConfig &config, const DeviceType &type, cstag_t &name, Device *owner, uint64_t clock)
: type(type), sysConfig(config), devName(name), owner(owner), clock(clock)
{
    ifaceList.clear();
}

void Device::addInterface(DeviceInterface *iface)
{
    ifaceList.push_back(iface);
}

Device *Device::findDevice(ctag_t *name)
{
    // Return this as self device
    if (name == nullptr)
        return this;

    // Find child device under this
    for (auto &dev : DeviceIterator(*this))
        if (name == dev.getDeviceName())
            return &dev;

    return nullptr;
}

void Device::configure(SystemConfig &config)
{
    assert (&config == &sysConfig);

    // Device initialization
    devConfigure(config);
}

void Device::finishConfig()
{
    for (auto *iface : ifaceList)
        iface->diCompleteConfig();
}

void Device::resolvePostMapping()
{
    findObjects();
    devResolveObjects();
}

cfwEntry_t *Device::getFirmwareEntries()
{
    static constexpr cfwEntry_t fwEmpty[] = { FW_END };

    if (fwEntries == nullptr)
    {
        fwEntries = devGetFirmwareEntries();
        if (fwEntries == nullptr)
            fwEntries = fwEmpty;
    } 

    return fwEntries;
}

map::MemoryRegion *Device::findMemoryRegion(ctag_t *name)
{
    assert(ownMachine != nullptr);
    return ownMachine->getMemoryManager().findRegion(name);
}

map::MemoryBank *Device::findMemoryBank(ctag_t *name)
{
    assert(ownMachine != nullptr);
    return ownMachine->getMemoryManager().findBank(name);
}

map::MemoryShare *Device::findMemoryShare(ctag_t *name)
{
    assert(ownMachine != nullptr);
    return ownMachine->getMemoryManager().findShare(name);
}

void Device::registerObject(ObjectFinder *object)
{
    objectList.push_back(object);
}

bool Device::findObjects()
{
    bool allFound = true;
    for (auto &object : objectList)
        allFound &= object->find();
    return false;   
}

// ********

DeviceInterface::DeviceInterface(Device *owner, ctag_t *name)
: owner(owner), diName(name)
{
    owner->addInterface(this);
}