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

    // Expand device name to path name
    if (owner != nullptr)
        pathName.assign(owner->getsPathName()).append(".").append(name);
    else
        pathName.assign(name);
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
        if (name == dev.getcDeviceName())
            return &dev;

    return nullptr;
}

std::string Device::expandPathName(cstag_t &pathName) const
{
    std::string newPathName(this->pathName);

    if (!pathName.empty())
    {
        if (pathName.find('.') != std::string::npos)
            newPathName.assign(pathName);
        else
            newPathName.append(".").append(pathName);
    }

    fmt::printf("Expend pathname: %s + %s -> %s\n",
        this->pathName, pathName.empty() ? "(self)" : pathName, newPathName);
    return newPathName;
}

void Device::configure(SystemConfig &config)
{
    assert (&config == &sysConfig);

    // Device initialization
    devConfigure(config);
}

void Device::start()
{
    devStart();
    // updateClock();

    flagStarted = true;
}

void Device::stop()
{
    devStop();
    flagStarted = false;
}

void Device::reset()
{
    devReset();
}

void Device::finishConfig()
{
    for (auto *iface : ifaceList)
        iface->diCompleteConfig();
}

void Device::updateClock()
{
    for (auto &iface : ifaceList)
        iface->diUpdateClock();
    devUpdateClock();
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

map::MemoryRegion *Device::findMemoryRegion(cstag_t &name) const
{
    assert(ownMachine != nullptr);
    return ownMachine->getMemoryManager().findRegion(expandPathName(name));
}

map::MemoryBank *Device::findMemoryBank(cstag_t &name) const
{
    assert(ownMachine != nullptr);
    return ownMachine->getMemoryManager().findBank(expandPathName(name));
}

map::MemoryShare *Device::findMemoryShare(cstag_t &name) const
{
    assert(ownMachine != nullptr);
    return ownMachine->getMemoryManager().findShare(expandPathName(name));
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