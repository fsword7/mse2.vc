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
    
protected:

    map::AddressConfig mapProgramConfig;

    map::MemoryAccess<16, 0, 0, LittleEndian>::specific mapProgram;

};

DECLARE_DEVICE_TYPE(upd7810, upd7810_cpuDevice);