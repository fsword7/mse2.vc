// vt100.h - DEC VT100 terminal emulator
//
// Author:  Tim Stark
// Date:    12/8/2021

#pragma once

class vt100_Device : public SystemDevice
{
public:
    vt100_Device(const SystemConfig &config, const DeviceType &type, cstag_t &devName, uint64_t clock)
    : SystemDevice(config, type, devName, clock),
      cpu(*this, "i8080"),
      crt(*this, "VT100_Video")
    {

    }

    void vt100(SystemConfig &config);
    void vt100_init();

    void vt100_setMemoryMap(map::AddressList &map);
    void vt100_setIOPort(map::AddressList &map);
    
private:
    RequiredDevice<i8080_cpuDevice> cpu;
    RequiredDevice<vt100video_t> crt;
};
