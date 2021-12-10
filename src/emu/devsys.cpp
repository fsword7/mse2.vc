// devsys.cpp - system device package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

#include "emu/core.h"
#include "emu/devsys.h"

SystemDevice::SystemDevice(const SystemConfig &config, const DeviceType &type, cstag_t &devName, uint64_t clock)
: Device(config, type, devName, nullptr, clock), driver(config.getSystemDriver())
{
    
}

void SystemDevice::devConfigure(SystemConfig &config)
{
    driver.configure(config, *this);
}