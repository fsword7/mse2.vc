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
      crt(*this, "VT100_Video"),
      ramData(*this, "ram")
    {

    }

    void vt100(SystemConfig &config);
    void vt100_init();

    void vt100_setMemoryMap(map::AddressList &map);
    void vt100_setIOPort(map::AddressList &map);
    
    uint32_t vt100_updateScreen(ScreenDevice &screen, bitmap16_t &bitmap, const rect_t &clip);
    uint8_t readData(offs_t addr);

private:
    RequiredDevice<i8080_cpuDevice> cpu;
    RequiredDevice<vt100video_t> crt;

    RequiredSharedPointer<uint8_t> ramData;
};
