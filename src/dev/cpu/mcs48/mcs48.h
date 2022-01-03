// mcs48.h - MCS 48 processor series package
//
// Author:  Tim Stark
// Date:    12/28/21

#pragma once

#include "emu/devcpu.h"

// PSW flag definitions
//
// +---+---+---+---+---+---+---+---+
// | C | A | F | B | 1 |     SP    |
// +---+---+---+---+---+---+---+---+

#define PSW_C		0x80	// Carry flag
#define PSW_A		0x40	// Half-carry flag
#define PSW_F		0x20	// F0 flag
#define PSW_B		0x10	// Register bank flag
#define PSW_MBZ		0x08	// Must be zero
#define PSW_SP		0x07	// Stack pointer in data space

// P2 flags for UPI-41 processors
#define P2_OBF		0x10
#define P2_NIBF		0x20
#define P2_DRQ		0x40
#define P2_NDACK	0x80

// STS register for UPI-41 processors
#define STS_IBF		0x02
#define STS_OBF		0x01

#define R0 iRegs[0]
#define R1 iRegs[1]
#define R2 iRegs[2]
#define R3 iRegs[3]
#define R4 iRegs[4]
#define R5 iRegs[5]
#define R6 iRegs[6]
#define R7 iRegs[7]

// MCS-48 feature defintions
#define CPUF_EXTERNAL_BUS	0x80
#define CPUF_MB				0x40
#define CPUF_UPI41			0x04
#define CPUF_802X			0x02
#define CPUF_804X			0x01

class mcs48_cpuDevice : public ProcessorDevice
{
public:
	// 8243 expander operations
	enum opExpander
	{
		eopRead  = 0,
		eopWrite = 1,
		eopOR    = 2,
		eopAND   = 3
	};

    mcs48_cpuDevice(const SystemConfig &config, const DeviceType &type,
        cstag_t &devName, Device *owner, uint64_t clock, int paWidth, int daWidth, int romSize);
    virtual ~mcs48_cpuDevice() = default;

	// Virtual device function calls
	void devStart() override;

protected:
    map::AddressConfigList getAddressConfigList() const override;
	
	void setProgram1024(map::AddressList &map);
	void setProgram2048(map::AddressList &map);
	void setProgram4096(map::AddressList &map);

	void setData64(map::AddressList &map);
	void setData128(map::AddressList &map);
	void setData256(map::AddressList &map);

	inline void eatCycles(int cycles) { cpuCycles -= cycles; }
	inline void updateRegisters() { /* iRegs = &idata[pswReg & PSW_B ? 24 : 0]; */ }
	// RequiredSharedPointer<uint8_t> idata;

	uint16_t iromSize; // Internal ROM size (1024, 2048 or 4096 bytes)
	uint16_t iramSize; // Internal RAM size (64, 128 or 256 bytes)
	
    map::AddressConfig mapProgramConfig;
    map::AddressConfig mapDataConfig;
    map::AddressConfig mapIOPortConfig;

    map::MemoryAccess<12, 0, 0, LittleEndian>::specific mapProgram;
    map::MemoryAccess<8, 0, 0, LittleEndian>::specific mapData;
    map::MemoryAccess<8, 0, 0, LittleEndian>::specific mapIOPort;

	uint8_t  cpuFlags;
	uint8_t  *iRegs;
	uint8_t	 aReg;
	uint8_t  pswReg;
	uint16_t pcReg;
	uint16_t a11Reg;
	uint16_t fpcBase; // faulting PC address

	uint8_t  p1Reg;
	uint8_t  p2Reg;
	bool     f1Reg;

	uint8_t  dbbiReg;
	uint8_t  dbboReg;
	uint8_t  stsReg;

	bool irqState;
	bool irqPolled;
	bool irqInProgress = false;
	bool xirqEnable;
	
	bool tirqEnable;
	bool timerOverflow;
	bool timerFlag;
	uint8_t timerReg;

	bool dmaEnable;
	bool flagsEnable;

	uint64_t cpuCycles;

	uint8_t read8i();
	uint8_t getP2Mask();

	// program space access function calls
	uint8_t read8p(offs_t addr)                { return mapProgram.read8(addr); }

	// data space access function calls
	uint8_t read8d(offs_t addr)                { return mapData.read8(addr); }
	void write8d(offs_t addr, uint8_t data)    { mapData.write8(addr, data); }

	// I/O space access function calls
	uint8_t read8io(offs_t addr)               { return mapIOPort.read8(addr); }
	void write8io(offs_t addr, uint8_t data)   { mapIOPort.write8(addr, data); }

	// port/bus (callback) function calls
	uint8_t read8port(offs_t addr)             { return 0; }
	void write8port(offs_t addr, uint8_t data) { }
	uint8_t read8bus()                         { return 0; }
	void write8bus(uint8_t data)               { }
	uint8_t read8test(offs_t addr)             { return 0; }
	void write8prog(int v)                     { }

	// Expansion operation (8243 expander chip)
	void expand(uint8_t port, opExpander op);

	// Excute function calls
	void exADD(uint8_t val);
	void exADC(uint8_t val);
	void exCALL(uint16_t addr);
	void exJUMP(uint16_t addr);
	void exJCC(bool flag);

	void pushPCPSW();
	void pullPCPSW();
	void pullPC();

	// Opcode function calls
	void illegal();

	void opADD_A_R0();
	void opADD_A_R1();
	void opADD_A_R2();
	void opADD_A_R3();
	void opADD_A_R4();
	void opADD_A_R5();
	void opADD_A_R6();
	void opADD_A_R7();
	void opADD_A_XR0();
	void opADD_A_XR1();
	void opADD_A_N();
	
	void opADC_A_R0();
	void opADC_A_R1();
	void opADC_A_R2();
	void opADC_A_R3();
	void opADC_A_R4();
	void opADC_A_R5();
	void opADC_A_R6();
	void opADC_A_R7();
	void opADC_A_XR0();
	void opADC_A_XR1();
	void opADC_A_N();

	void opANL_A_R0();
	void opANL_A_R1();
	void opANL_A_R2();
	void opANL_A_R3();
	void opANL_A_R4();
	void opANL_A_R5();
	void opANL_A_R6();
	void opANL_A_R7();
	void opANL_A_XR0();
	void opANL_A_XR1();
	void opANL_A_N();

	void opANL_BUS_N();
	void opANL_P1_N();
	void opANL_P2_N();
	void opANLD_P4_A();
	void opANLD_P5_A();
	void opANLD_P6_A();
	void opANLD_P7_A();

	void opCALL_0();
	void opCALL_1();
	void opCALL_2();
	void opCALL_3();
	void opCALL_4();
	void opCALL_5();
	void opCALL_6();
	void opCALL_7();

	void opCLR_A();
	void opCLR_C();
	void opCLR_F0();
	void opCLR_F1();

	void opCPL_A();
	void opCPL_C();
	void opCPL_F0();
	void opCPL_F1();

	void opDA_A();

	void opDEC_A();
	void opDEC_R0();
	void opDEC_R1();
	void opDEC_R2();
	void opDEC_R3();
	void opDEC_R4();
	void opDEC_R5();
	void opDEC_R6();
	void opDEC_R7();

	void opDIS_I();
	void opDIS_TCNTI();

	void opDJNZ_R0();
	void opDJNZ_R1();
	void opDJNZ_R2();
	void opDJNZ_R3();
	void opDJNZ_R4();
	void opDJNZ_R5();
	void opDJNZ_R6();
	void opDJNZ_R7();

	void opEN_I();
	void opEN_TCNTI();
	void opEN_DMA();
	void opEN_FLAGS();
	void opENT0_CLK();

	void opIN_A_P0();
	void opIN_A_P1();
	void opIN_A_P2();
	void opINS_A_BUS();
	void opIN_A_DBB();

	void opINC_A();
	void opINC_R0();
	void opINC_R1();
	void opINC_R2();
	void opINC_R3();
	void opINC_R4();
	void opINC_R5();
	void opINC_R6();
	void opINC_R7();
	void opINC_XR0();
	void opINC_XR1();

	void opJB_0();
	void opJB_1();
	void opJB_2();
	void opJB_3();
	void opJB_4();
	void opJB_5();
	void opJB_6();
	void opJB_7();
	void opJC();
	void opJF0();
	void opJF1();
	void opJNC();
	void opJNI();
	void opJNIBF();
	void opJNT_0();
	void opJNT_1();
	void opJNZ();
	void opJOBF();
	void opJTF();
	void opJT_0();
	void opJT_1();
	void opJZ();

	void opJMP_0();
	void opJMP_1();
	void opJMP_2();
	void opJMP_3();
	void opJMP_4();
	void opJMP_5();
	void opJMP_6();
	void opJMP_7();
	void opJMPP_XA();

	void opMOV_A_N();
	void opMOV_A_PSW();
	void opMOV_A_R0();
	void opMOV_A_R1();	
	void opMOV_A_R2();
	void opMOV_A_R3();
	void opMOV_A_R4();
	void opMOV_A_R5();
	void opMOV_A_R6();
	void opMOV_A_R7();
	void opMOV_A_XR0();
	void opMOV_A_XR1();
	void opMOV_A_T();
	
	void opMOV_PSW_A();
	void opMOV_STS_A();
	void opMOV_R0_A();
	void opMOV_R1_A();
	void opMOV_R2_A();
	void opMOV_R3_A();
	void opMOV_R4_A();
	void opMOV_R5_A();
	void opMOV_R6_A();
	void opMOV_R7_A();
	void opMOV_R0_N();
	void opMOV_R1_N();
	void opMOV_R2_N();
	void opMOV_R3_N();
	void opMOV_R4_N();
	void opMOV_R5_N();
	void opMOV_R6_N();
	void opMOV_R7_N();
	void opMOV_T_A();
	void opMOV_XR0_A();
	void opMOV_XR1_A();
	void opMOV_XR0_N();
	void opMOV_XR1_N();

	void opMOVD_A_P4();
	void opMOVD_A_P5();
	void opMOVD_A_P6();
	void opMOVD_A_P7();
	void opMOVD_P4_A();
	void opMOVD_P5_A();
	void opMOVD_P6_A();
	void opMOVD_P7_A();

	void opMOVP_A_XA();
	void opMOVP3_A_XA();

	void opMOVX_A_XR0();
	void opMOVX_A_XR1();
	void opMOVX_XR0_A();
	void opMOVX_XR1_A();

	void opNOP();

	void opORL_A_R0();
	void opORL_A_R1();
	void opORL_A_R2();
	void opORL_A_R3();
	void opORL_A_R4();
	void opORL_A_R5();
	void opORL_A_R6();
	void opORL_A_R7();
	void opORL_A_XR0();
	void opORL_A_XR1();
	void opORL_A_N();

	void opORL_BUS_N();
	void opORL_P1_N();
	void opORL_P2_N();
	void opORLD_P4_A();
	void opORLD_P5_A();
	void opORLD_P6_A();
	void opORLD_P7_A();

	void opOUTL_BUS_A();
	void opOUTL_P0_A();
	void opOUTL_P1_A();
	void opOUTL_P2_A();
	void opOUT_DBB_A();

	void opRET();
	void opRETR();

	void opRL_A();
	void opRLC_A();
	void opRR_A();
	void opRRC_A();

	void opSEL_MB0();
	void opSEL_MB1();
	void opSEL_RB0();
	void opSEL_RB1();

	void opSTOP_TCNT();
	void opSTOP_T();
	void opSTRT_CNT();

	void opSWAP_A();

	void opXCH_A_R0();
	void opXCH_A_R1();
	void opXCH_A_R2();
	void opXCH_A_R3();
	void opXCH_A_R4();
	void opXCH_A_R5();
	void opXCH_A_R6();
	void opXCH_A_R7();
	void opXCH_A_XR0();
	void opXCH_A_XR1();

	void opXCHD_A_XR0();
	void opXCHD_A_XR1();

	void opXRL_A_R0();
	void opXRL_A_R1();
	void opXRL_A_R2();
	void opXRL_A_R3();
	void opXRL_A_R4();
	void opXRL_A_R5();
	void opXRL_A_R6();
	void opXRL_A_R7();
	void opXRL_A_XR0();
	void opXRL_A_XR1();
	void opXRL_A_N();
};

class i8021_cpuDevice : public mcs48_cpuDevice
{
public:
	i8021_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8021_cpuDevice() = default;
};

class i8022_cpuDevice : public mcs48_cpuDevice
{
public:
	i8022_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8022_cpuDevice() = default;
};

class i8035_cpuDevice : public mcs48_cpuDevice
{
public:
	i8035_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8035_cpuDevice() = default;
};

class i8039_cpuDevice : public mcs48_cpuDevice
{
public:
	i8039_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8039_cpuDevice() = default;
};

class i8040_cpuDevice : public mcs48_cpuDevice
{
public:
	i8040_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8040_cpuDevice() = default;
};

class i8048_cpuDevice : public mcs48_cpuDevice
{
public:
	i8048_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8048_cpuDevice() = default;
};

class i8648_cpuDevice : public mcs48_cpuDevice
{
public:
	i8648_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8648_cpuDevice() = default;
};

class i8748_cpuDevice : public mcs48_cpuDevice
{
public:
	i8748_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8748_cpuDevice() = default;
};

class i8049_cpuDevice : public mcs48_cpuDevice
{
public:
	i8049_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8049_cpuDevice() = default;
};

class i8749_cpuDevice : public mcs48_cpuDevice
{
public:
	i8749_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8749_cpuDevice() = default;
};

class i8050_cpuDevice : public mcs48_cpuDevice
{
public:
	i8050_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8050_cpuDevice() = default;
};

class i8750_cpuDevice : public mcs48_cpuDevice
{
public:
	i8750_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8750_cpuDevice() = default;
};


// Intel UPI-41 processor series

class upi41_cpuDevice : public mcs48_cpuDevice
{
protected:
	upi41_cpuDevice(const SystemConfig &config, const DeviceType &type, cstag_t &devName,
		Device *owner, uint64_t clock, int paWidth, int daWidth, int romSize)
	: mcs48_cpuDevice(config, type, devName, owner, clock, paWidth, daWidth, romSize)
	{ }
};

class i8041a_cpuDevice : public upi41_cpuDevice
{
public:
	i8041a_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
};

class i8741a_cpuDevice : public upi41_cpuDevice
{
public:
	i8741a_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
};

class i8041ah_cpuDevice : public upi41_cpuDevice
{
public:
	i8041ah_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
};

class i8741ah_cpuDevice : public upi41_cpuDevice
{
public:
	i8741ah_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
};

class i8042_cpuDevice : public upi41_cpuDevice
{
public:
	i8042_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
};

class i8742_cpuDevice : public upi41_cpuDevice
{
public:
	i8742_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
};

class i8042ah_cpuDevice : public upi41_cpuDevice
{
public:
	i8042ah_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
};

class i8742ah_cpuDevice : public upi41_cpuDevice
{
public:
	i8742ah_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
};

// Intel MCS-48 processors
DECLARE_DEVICE_TYPE(I8021,   i8021_cpuDevice);
DECLARE_DEVICE_TYPE(I8022,   i8022_cpuDevice);
DECLARE_DEVICE_TYPE(I8035,   i8035_cpuDevice);
DECLARE_DEVICE_TYPE(I8039,   i8039_cpuDevice);
DECLARE_DEVICE_TYPE(I8040,   i8040_cpuDevice);
DECLARE_DEVICE_TYPE(I8048,   i8048_cpuDevice);
DECLARE_DEVICE_TYPE(I8648,   i8648_cpuDevice);
DECLARE_DEVICE_TYPE(I8748,   i8748_cpuDevice);
DECLARE_DEVICE_TYPE(I8049,   i8049_cpuDevice);
DECLARE_DEVICE_TYPE(I8749,   i8749_cpuDevice);
DECLARE_DEVICE_TYPE(I8050,   i8050_cpuDevice);
DECLARE_DEVICE_TYPE(I8750,   i8750_cpuDevice);

// Intel UPI-41 processors
DECLARE_DEVICE_TYPE(I8041A,  i8041a_cpuDevice);
DECLARE_DEVICE_TYPE(I8741A,  i8741a_cpuDevice);
DECLARE_DEVICE_TYPE(I8041AH, i8041ah_cpuDevice);
DECLARE_DEVICE_TYPE(I8741AH, i8741ah_cpuDevice);
DECLARE_DEVICE_TYPE(I8042,   i8042_cpuDevice);
DECLARE_DEVICE_TYPE(I8742,   i8742_cpuDevice);
DECLARE_DEVICE_TYPE(I8042AH, i8042ah_cpuDevice);
DECLARE_DEVICE_TYPE(I8742AH, i8742ah_cpuDevice);