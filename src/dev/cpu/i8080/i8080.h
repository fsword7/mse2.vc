// i8080.h - Intel 8080/8085 Processor package
//
// Author:  Tim Stark
// Date:    12/10/2021

#pragma once

#include "emu/devcpu.h"

// status register definition
#define PSW_SF   0x80   // Signed bit
#define PSW_ZF   0x40   // Zero bit
#define PSW_X5F  0x20
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

// 8-bit registers
#if LSB_FIRST
// for little-endian host systems
#define REGn_A  0
#define REGn_F  1
#define REGn_B  2
#define REGn_C  3
#define REGn_D  4
#define REGn_E  5
#define REGn_H  6
#define REGn_L  7
#define REGn_M  REGn_F // M - indirect HL register
#else
// for big-endian host systems
#define REGn_A  1
#define REGn_F  0
#define REGn_B  3
#define REGn_C  2
#define REGn_D  5
#define REGn_E  4
#define REGn_H  7
#define REGn_L  6
#define REGn_M  REGn_F // M - indirect HL register
#endif

// 16-bit registers
#define REGn_AF 0
#define REGn_BC 1
#define REGn_DE 2
#define REGn_HL 3
#define REGn_SP 4
#define REGn_PC 5

// Opcode field macro definitions
#define REGAn(opCode)   (opCode & 0x07)
#define REGBn(opCode)   ((opCode >> 3) & 0x07)
#define REGWn(opCode)   ((opCode >> 4) & 0x03)

#define REGA(opCode)    state.bRegs[REGAn(opCode)+2]
#define REGB(opCode)    state.bRegs[REGBn(opCode)+2]
#define REGW(opCode)    state.wRegs[REGWn(opCode)+1]

#define REG_A           state.bRegs[REGn_A]
#define REG_F           state.bRegs[REGn_F]

#define REG_AF          state.wRegs[REGn_AF]
#define REG_BC          state.wRegs[REGn_BC]
#define REG_DE          state.wRegs[REGn_DE]
#define REG_HL          state.wRegs[REGn_HL]
#define REG_SP          state.wRegs[REGn_SP]
#define REG_PC          state.wRegs[REGn_PC]

class i8080_cpuDevice : public ProcessorDevice
{
public:
    enum cpuType
    {
        cpuid_8080,
        cpuid_8080A,
        cpuid_8085
    };

    enum stateRegisters
    {
        i8080_A,  i8080_F,  i8080_B,  i8080_C,
        i8080_D,  i8080_E,  i8080_H,  i8080_L,
        i8080_AF, i8080_BC, i8080_DE, i8080_HL,
        i8080_SP, i8080_PC
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
    uint16_t readi16();
    uint8_t read8(offs_t addr);
    void write8(offs_t addr, uint8_t data);

    void execute();

    void enableInterrupts(bool enable);

    void opPUSH(uint16_t val);
    uint16_t opPOP();

    void opRST(uint8_t val);
    void opCALL(bool flag);
    void opJMP(bool flag);
    void opRET(bool flag);

    uint8_t opINR(uint8_t val);
    uint8_t opDCR(uint8_t val);
    
    void opDAD(uint16_t val);
    void opADD(uint8_t val);
    void opADC(uint8_t val);
    void opSUB(uint8_t val);
    void opSBB(uint8_t val);
    void opANA(uint8_t val);
    void opORA(uint8_t val);
    void opXRA(uint8_t val);
    void opCMP(uint8_t val);


    cpuType idType;

    union {
        uint8_t  bRegs[8];
        uint16_t wRegs[6];
    } state;

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