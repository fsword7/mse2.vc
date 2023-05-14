// device.cpp - device management package
//
// Date:    Apr 30, 2023
// Author:  Tim Stark

#include "emu/core.h"

Device::Device(const SystemConfig &config, cDeviceType &type, cstr_t &name, Device *owner, uint64_t clock)
: sysConfig(config), type(type), devName(name), owner(owner), clock(clock)
{
    // ifaceList.clear();

    // // Expand device name to path name
    // if (owner != nullptr)
    //     pathName.assign(owner->getsPathName()).append(".").append(name);
    // else
    //     pathName.assign(name);
}

void Device::configure(SystemConfig &config)
{
    assert(&config == &sysConfig);

    // Device initialization
    // devConfigure(config);   
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