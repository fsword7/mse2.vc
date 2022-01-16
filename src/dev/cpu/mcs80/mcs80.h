// mcs80.h - Intel 8080/8085 Processor package
//
// Author:  Tim Stark
// Date:    Dec 10, 2021

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
#define REGn_A  1
#define REGn_F  0
#define REGn_B  3
#define REGn_C  2
#define REGn_D  5
#define REGn_E  4
#define REGn_H  7
#define REGn_L  6
// #define REGn_M  REGn_F // M - indirect HL register
#else
// for big-endian host systems
#define REGn_A  0
#define REGn_F  1
#define REGn_B  2
#define REGn_C  3
#define REGn_D  4
#define REGn_E  5
#define REGn_H  6
#define REGn_L  7
// #define REGn_M  REGn_F // M - indirect HL register
#endif

// 16-bit registers
#define REGn_AF 0
#define REGn_BC 1
#define REGn_DE 2
#define REGn_HL 3
#define REGn_SP 4
#define REGn_PC 5

// Opcode field macro definitions
#define REGAn(opCode)   ((opCode & 0x07) ^ 1)
#define REGBn(opCode)   (((opCode >> 3) & 0x07) ^ 1)
#define REGWn(opCode)   ((opCode >> 4) & 0x03)

#define REGA(opCode)    state.bRegs[REGAn(opCode) < 6 ? REGAn(opCode)+2 : 1]
#define REGB(opCode)    state.bRegs[REGBn(opCode) < 6 ? REGBn(opCode)+2 : 1]
#define REGW(opCode)    state.wRegs[REGWn(opCode)+1]

#define REG_A           state.bRegs[REGn_A]
#define REG_F           state.bRegs[REGn_F]
#define REG_B           state.bRegs[REGn_B]
#define REG_C           state.bRegs[REGn_C]
#define REG_D           state.bRegs[REGn_D]
#define REG_E           state.bRegs[REGn_E]
#define REG_H           state.bRegs[REGn_H]
#define REG_L           state.bRegs[REGn_L]

#define REG_AF          state.wRegs[REGn_AF]
#define REG_BC          state.wRegs[REGn_BC]
#define REG_DE          state.wRegs[REGn_DE]
#define REG_HL          state.wRegs[REGn_HL]
#define REG_SP          state.wRegs[REGn_SP]
#define REG_PC          state.wRegs[REGn_PC]

#define OPR_IMPLIED     0
#define OPR_RS8         1
#define OPR_RD8         2
#define OPR_RSD8        3
#define OPR_REG16S      4
#define OPR_REG16P      5
#define OPR_REGI8       6
#define OPR_REGI16      7
#define OPR_IMM8        8
#define OPR_ADDR        9
#define OPR_VECTOR      10
#define OPR_PORT        11

struct mcs80_opCode_t
{
    cchar_t     *opName;
    uint8_t     opCode;
    uint8_t     opMask;
    uint8_t     opType;
};

class mcs80_cpuDevice : public ProcessorDevice
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

    mcs80_cpuDevice(const SystemConfig &config, cstag_t &devName,
        Device *owner, uint64_t clock);
    virtual ~mcs80_cpuDevice() = default;

    // Virtual device function calls
    void devStart() override;

    uint64_t executeClockToCycle(uint64_t clocks) const override { return (clocks + 2 - 1) / 2; }
    uint64_t executeCycleToClock(uint64_t cycles) const override { return (cycles * 2); }
    uint64_t executeGetMinCycles() const override { return 4; }
    uint64_t executeGetMaxCycles() const override { return 16; }
    // int executeGetInputLines()     { return 4; }

    void step() override;
    void executeRun() override;

    int list(offs_t vAddr) override;

protected:
    mcs80_cpuDevice(const SystemConfig &config, const DeviceType &type,
        cstag_t &devName, Device *owner, uint64_t clock, cpuType idType);

    map::AddressConfigList getAddressConfigList() const override;

    void init();
    void reset();

    void initOpcodeTable();

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
    int64_t cpuCycles = 0;

    uint8_t zspFlags[256]; // Z/P flag table

    map::AddressConfig mapProgramConfig;
    map::AddressConfig mapIOPortConfig;

    map::MemoryAccess<16, 0, 0, LittleEndian>::specific mapProgram;
    map::MemoryAccess<8, 0, 0, LittleEndian>::specific mapIOPort;

    static const mcs80_opCode_t opTable[];

    const mcs80_opCode_t *opCodes[256];
};

class mcs80a_cpuDevice : public mcs80_cpuDevice
{
public:
	mcs80a_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~mcs80a_cpuDevice() = default;

};

class mcs85_cpuDevice : public mcs80_cpuDevice
{
public:
	mcs85_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~mcs85_cpuDevice() = default;

};

DECLARE_DEVICE_TYPE(I8080, mcs80_cpuDevice);
DECLARE_DEVICE_TYPE(I8080A, mcs80a_cpuDevice);
DECLARE_DEVICE_TYPE(I8085, mcs85_cpuDevice);