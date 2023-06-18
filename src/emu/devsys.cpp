// devsys.cpp - system device package
//
// Date:    May 2, 2023
// Author:  Tim Stark

#include "emu/core.h"
#include "emu/devsys.h"

SystemDevice::SystemDevice(SystemConfig &config, cDeviceType &type, cstr_t &sysName, uint64_t clock)
: Device(config, type, sysName, nullptr, clock),
  driver(config.getSystemDriver())
{

}

void SystemDevice::devConfigure(SystemConfig &config)
{
    driver.configure(config, *this);
}