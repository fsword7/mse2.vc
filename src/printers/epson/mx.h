// mx.h - Epson MX80/MX100 dot matrix printer
//
// Author:  Tim Stark
// Date:    12/28/21

#pragma once

class mx80_Device : public SystemDevice
{
public:
    mx80_Device(const SystemConfig &config, const DeviceType &type, cstag_t &devName, uint64_t clock)
    : SystemDevice(config, type, devName, clock),
      pmcu(*this, "pmcu"), smcu(*this, "smcu")
    {

    }

    void mx80(SystemConfig &config);
    void mx80_init();

    void mx100(SystemConfig &config);
    void mx100_init();

    void mx80_setMemoryMap(map::AddressList &map);
    // void mx80_setDataMap(map::AddressList &map);
    // void mx80_setIOPort(map::AddressList &map);

private:
    RequiredDevice<i8039_cpuDevice> pmcu;  // primary microcontroller
    RequiredDevice<i8041a_cpuDevice> smcu; // slave microcontroller
};
