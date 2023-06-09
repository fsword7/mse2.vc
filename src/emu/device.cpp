// device.cpp - device management package
//
// Date:    Apr 30, 2023
// Author:  Tim Stark

#include "emu/core.h"
#include "emu/machine.h"

Device::Device(SystemConfig &config, cDeviceType &type, cstr_t &name, Device *owner, uint64_t clock)
: sysConfig(config), type(type), ownMachine(config.getMachine()),
  devName(name), owner(owner), clock(clock)
{
    ifaceList.clear();

    // // Expand device name to path name
    if (owner != nullptr)
        pathName.assign(owner->getsPathName()).append(".").append(name);
    else
        pathName.assign(name);
}

str_t Device::expandPathName(cstr_t &pathName) const
{
    str_t newPathName(this->pathName);

    if (!pathName.empty())
    {
        if (pathName.find('.') != std::string::npos)
            newPathName.assign(pathName);
        else
            newPathName.append(".").append(pathName);
    }

    std::cout << fmt::format("Expend pathname: {} + {} -> {}\n",
        this->pathName, pathName.empty() ? "(self)" : pathName, newPathName);
    return newPathName;
}

Device *Device::findDevice(cchar_t *name)
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

void Device::configure(SystemConfig &config)
{
    assert(&config == &sysConfig);

    // Device initialization
    devConfigure(config);   
}

void Device::start()
{
    devStart();
    startedFlag = true;
}

void Device::stop()
{
    devStop();
    startedFlag = false;
}

void Device::reset()
{
    devReset();
}

// **** Mapping function calls ****

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

void Device::resolveFinalMapping()
{
    findObjects();
}

map::MemoryRegion *Device::findMemoryRegion(cstr_t &name) const
{
    return ownMachine.getMemoryManager().findRegion(expandPathName(name));
}

map::MemoryBank *Device::findMemoryBank(cstr_t &name) const
{
    return ownMachine.getMemoryManager().findBank(expandPathName(name));
}

map::MemoryShare *Device::findMemoryShare(cstr_t &name) const
{
    return ownMachine.getMemoryManager().findShare(expandPathName(name));
}

// **** Device Interface function calls ****

void Device::addInterface(DeviceInterface *iface)
{
    ifaceList.push_back(iface);
}

void Device::finishConfig()
{
    for (auto *iface : ifaceList)
        iface->diCompleteConfig();
}

// ********

DeviceInterface::DeviceInterface(Device *owner, cstr_t &name)
: owner(owner), diName(name)
{
    owner->addInterface(this);
}