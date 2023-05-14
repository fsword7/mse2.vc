// devcpu.h - CPU Device Package
//
// Date:    May 8, 2023
// Author:  Tim Stark

#pragma once
class CPUDevice : public Device
{
public:
    CPUDevice(const SystemConfig &config, cDeviceType &type,
        cstr_t &devName, Device *owner, uint64_t clock);

};