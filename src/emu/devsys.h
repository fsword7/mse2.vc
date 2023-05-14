// devsys.h - system device package
//
// Date:    May 2, 2023
// Author:  Tim Stark

#pragma once
class SystemDevice : public Device
{
public:
    SystemDevice(const SystemConfig &config, cDeviceType &type, cstr_t &sysName, uint64_t clock);

};