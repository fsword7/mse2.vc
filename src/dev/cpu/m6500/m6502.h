// m6502.h - MOS 6502 microprocessor package
//
// Author:  Tim Stark
// Date:    Jan 15, 2022

#include "emu/devcpu.h"

class m6502_cpuDevice : public ProcessorDevice
{
public:
    m6502_cpuDevice(const SystemConfig &config, cstag_t &devName,
        Device *owner, uint64_t clock);
    virtual ~m6502_cpuDevice() = default;

    // Virtual device function calls
    void devStart() override;

    // uint64_t executeClockToCycle(uint64_t clocks) const override { return (clocks + 2 - 1) / 2; }
    // uint64_t executeCycleToClock(uint64_t cycles) const override { return (cycles * 2); }
    uint64_t executeGetMinCycles() const override { return 1; }
    uint64_t executeGetMaxCycles() const override { return 16; }
    // int executeGetInputLines()     { return 4; }


protected:
    m6502_cpuDevice(const SystemConfig &config, const DeviceType &type,
        cstag_t &devName, Device *owner, uint64_t clock);

    map::AddressConfigList getAddressConfigList() const override;

    map::AddressConfig mapProgramConfig;

    map::MemoryAccess<16, 0, 0, LittleEndian>::specific mapProgram;

    int64_t cpuCycles;

};

DECLARE_DEVICE_TYPE(M6502, m6502_cpuDevice);
