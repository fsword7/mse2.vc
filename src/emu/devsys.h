// devsys.h - system device package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

#pragma once

class SystemDevice : public Device
{
public:
    SystemDevice(const SystemConfig &config, const DeviceType &type, cstag_t &devName, uint64_t clock);

    void devConfigure(SystemConfig &config) override;
    cfwEntry_t *devGetFirmwareEntries() { return driver.romEntries; }
    
private:
    const SystemDriver &driver;
};