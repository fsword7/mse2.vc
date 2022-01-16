// upd7810.h - NEC uPD7810 processor package
//
// Author:  Tim Stark
// Date:    Jan 9, 2022

#pragma once

#include "emu/devcpu.h"

class upd7810_cpuDevice : public ProcessorDevice
{
protected:
    upd7810_cpuDevice(const SystemConfig &config, const DeviceType &type,
        cstag_t &devName, Device *owner, uint64_t clock);

public:
    upd7810_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
    virtual ~upd7810_cpuDevice() = default;

    map::AddressConfigList getAddressConfigList() const;

    void devStart() override;

    uint64_t executeClockToCycle(uint64_t clocks) const override { return (clocks + 3 - 1) / 3; }
    uint64_t executeCycleToClock(uint64_t cycles) const override { return (cycles * 3); }
    uint64_t executeGetMinCycles() const override { return 1; }
    uint64_t executeGetMaxCycles() const override { return 40; }
    // int executeGetInputLines()     { return 2; }

protected:
    void setData128(map::AddressList &map);
    void setData256(map::AddressList &map);
    // void setROM4096(map::AddressList &map);

    map::AddressConfig mapProgramConfig;

    map::MemoryAccess<16, 0, 0, LittleEndian>::specific mapProgram;

};

DECLARE_DEVICE_TYPE(UPD7810, upd7810_cpuDevice);