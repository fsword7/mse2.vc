// sysconfig.cpp - system configuration package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    Dec 6, 2021

#include "emu/core.h"

SystemConfig::SystemConfig(const SystemDriver &driver, cstag_t &name)
{
    addDeviceType(driver.type, name, 0);
}

Device *SystemConfig::addDeviceType(const DeviceType &type, cstag_t &name, uint64_t clock)
{
    Device *owner = nullptr;

    Device *sys = type.create(*this, name, nullptr, clock);
    return addDevice(sys, owner);
}

Device *SystemConfig::addDevice(Device *dev, Device *owner)
{

    if (owner != nullptr)
    {
        owner->addNode(dev);
    }
    else
    {
        // System device
        assert(sysDevice == nullptr);
        sysDevice = dev;
    }

    // Initialize device
    dev->configure(*this);

    return dev;
}