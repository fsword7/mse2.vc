// sysconfig.cpp - system configuration package
//
// Date:    May 2, 2023
// Author:  Tim Stark

#include "emu/core.h"

SystemConfig::SystemConfig(cSystemDriver &driver, cstr_t &name, Machine &machine)
: driver(driver), ownMachine(machine)
{
    createSystemDevice(driver.type, name, 0);

    // Complete final configuration
    for (auto &dev : DeviceIterator(*sysDevice))
    {
        std::cout << fmt::format("{}: completing final configuration\n", dev.getsDeviceName());
        dev.finishConfig();
    }
}

Device *SystemConfig::createSystemDevice(const DeviceType &type, cstr_t &name, uint64_t clock)
{
    Device *owner = nullptr;

    std::cout << fmt::format("{}: Creating {} system...\n", name, type.getShortName());

    Device *sys = type.create(*this, name, nullptr, clock);
    return addDevice(sys, owner);
}

Device *SystemConfig::addDevice(Device *dev, Device *owner)
{
    assert (dev != nullptr);

    if (owner != nullptr)
    {
        // Adding device under owning device
        std::cout << fmt::format("{}: Adding {}({}) device\n", owner->getsDeviceName(),
            dev->getsDeviceName(), dev->getShortName());
        owner->addNode(dev);
    }
    else
    {
        // Set system device
        assert(sysDevice == nullptr);
        sysDevice = dev;
    }

    // Initialize configuring device
    std::cout << fmt::format("{}: Start device configuration...\n", dev->getsDeviceName());
    cfgDevice.push(dev);
    curDevice = dev;

    dev->configure(*this);

    cfgDevice.pop();
    return dev;
}

void SystemConfig::setPerfectQuantum(Device &device, cstr_t &devName)
{

    perfectQuantumDevice.first = &device;
    perfectQuantumDevice.second = std::move(devName);
}