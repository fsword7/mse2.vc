// device.cpp - Device/Interface package
//
// Author:  Tim Stark
// Date:    Dec 6, 2021

#include "emu/core.h"

Device::Device(const SystemConfig &config, const DeviceType &type, cstag_t &name, Device *owner, uint64_t clock)
: type(type), sysConfig(config), devName(name), owner(owner), clock(clock)
{

}

void Device::configure(SystemConfig &config)
{
    assert (&config == &sysConfig);

    // Device initialization
    devConfigure(config);
}