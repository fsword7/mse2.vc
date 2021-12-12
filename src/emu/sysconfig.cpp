// sysconfig.cpp - system configuration package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    Dec 6, 2021

#include "emu/core.h"

SystemConfig::SystemConfig(const SystemDriver &driver, cstag_t &name)
: driver(driver)
{
    createSystemDevice(driver.type, name, 0);

    // Complete final configuration
    for (auto &dev : DeviceIterator(*sysDevice))
    {
        fmt::printf("%s: completing final configuration\n", dev.getDeviceName());
        dev.finishConfig();
    }
}

Device *SystemConfig::createSystemDevice(const DeviceType &type, cstag_t &name, uint64_t clock)
{
    Device *owner = nullptr;

    fmt::printf("%s: Creating %s system...\n", name, type.getShortName());

    Device *sys = type.create(*this, name, nullptr, clock);
    return addDevice(sys, owner);
}

Device *SystemConfig::addDevice(Device *dev, Device *owner)
{
    assert (dev != nullptr);

    if (owner != nullptr)
    {
        // Adding device under owning device
        fmt::printf("%s: Adding %s(%s) device\n", owner->getDeviceName(),
            dev->getDeviceName(), dev->getShortName());
        owner->addNode(dev);
    }
    else
    {
        // Set system device
        assert(sysDevice == nullptr);
        sysDevice = dev;
    }

    // Initialize configuring device
    fmt::printf("%s: Start device configuration...\n", dev->getDeviceName());
    cfgDevice.push(dev);
    dev->configure(*this);

    cfgDevice.pop();
    return dev;
}