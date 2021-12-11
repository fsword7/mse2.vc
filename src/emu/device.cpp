// device.cpp - Device/Interface package
//
// Author:  Tim Stark
// Date:    Dec 6, 2021

#include "emu/core.h"

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

void Device::registerObject(ObjectFinder *object)
{
    objectList.push_back(object);
}

bool Device::findObjects()
{
    return false;   
}