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
      pmcu(*this, "pmcu"), smcu(*this, "smcu"), i8155(*this, "ramio")
    {

    }

    void mx80(SystemConfig &config);
    void mx80_init();

    void mx100(SystemConfig &config);
    void mx100_init();

    void mx80_setProgram(map::AddressList &map);
    void mx80_setExtData(map::AddressList &map);

private:
    uint8_t read8io(offs_t offset);
    void write8io(offs_t offset, uint8_t data);

    RequiredDevice<i8039_cpuDevice> pmcu;   // primary microcontroller
    RequiredDevice<i8041a_cpuDevice> smcu;  // slave microcontroller
    RequiredDevice<i8155_Device> i8155;     // 8155 RAM, I/O and timer chip

    uint8_t p1Flags;
    uint8_t p2Flags;
    uint8_t sw1Flags;
    uint8_t sw2Flags;
};
