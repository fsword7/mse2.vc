// i8080.h - Intel 8080/8085 Processor package
//
// Author:  Tim Stark
// Date:    12/10/2021

#pragma once

#include "emu/devcpu.h"

// status register definition
#define PSW_SF   0x80   // Signed bit
#define PSW_ZF   0x40   // Zero bit
#define PSW_XSF  0x20
#define PSW_HF   0x10   // Half-carry bit
#define PSW_X3F  0x08
#define PSW_PF   0x04   // Odd-parity bit
#define PSW_VF   0x02   // Overflow bit
#define PSW_CF   0x01   // Carry bit

// 8/16-bit register definition
//
// +-----+-----+
// |  A  |  F  |
// +-----+-----+
// |  B  |  C  |
// +-----+-----+
// |  D  |  E  |
// +-----+-----+
// |  H  |  L  |
// +-----+-----+
// |     SP    |
// +-----------+
// |     PC    |
// +-----------+

#define REGAn(opCode)   (opCode & 0x07)
#define REGBn(opCode)   ((opCode >> 3) & 0x07)
#define REGWn(opCode)   ((opCode >> 4) & 0x03)

class i8080_cpuDevice : public ProcessorDevice
{
public:
    enum cpuType
    {
        cpuid_8080,
        cpuid_8080A,
        cpuid_8085
    };

    i8080_cpuDevice(const SystemConfig &config, const DeviceType &type,
        cstag_t &devName, Device *owner, uint64_t clock, cpuType idType);
    i8080_cpuDevice(const SystemConfig &config, cstag_t &devName,
        Device *owner, uint64_t clock);
    virtual ~i8080_cpuDevice() = default;

protected:
    map::AddressConfigList getAddressConfigList() const override;

    void init();

    inline bool is8080() { return idType == cpuid_8080 || idType == cpuid_8080A; }
    inline bool is8085() { return idType == cpuid_8085; }

    // read/write access function calls
    uint8_t readi8();
    pair16_t readi16();
    uint8_t read8(offs_t addr);
    void write8(offs_t addr, uint8_t data);

    void execute();

    cpuType idType;

    // General-purpose 8/16-bit registers
    pair16_t afReg; // A/F register
    pair16_t bcReg; // B/C register
    pair16_t deReg; // D/E register
    pair16_t hlReg; // H/L register
    pair16_t pcReg; // PC register
    pair16_t spReg; // SP register
    pair16_t wzReg;
    uint16_t pcBase; // current base PC address

    const uint8_t *opCycleTable = nullptr;
    int64_t opCount = 0;

    uint8_t zspFlags[256]; // Z/P flag table

    map::AddressConfig mapProgramConfig;
    map::AddressConfig mapIOPortConfig;

    map::MemoryAccess<16, 0, 0, LittleEndian>::specific mapProgram;
    map::MemoryAccess<8, 0, 0, LittleEndian>::specific mapIOPort;
};

class i8080a_cpuDevice : public i8080_cpuDevice
{
public:
	i8080a_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8080a_cpuDevice() = default;

};

class i8085_cpuDevice : public i8080_cpuDevice
{
public:
	i8085_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8085_cpuDevice() = default;

};

DECLARE_DEVICE_TYPE(i8080, i8080_cpuDevice);
DECLARE_DEVICE_TYPE(i8080a, i8080a_cpuDevice);
DECLARE_DEVICE_TYPE(i8085, i8085_cpuDevice);