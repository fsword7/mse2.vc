// axpcpu.h - Alpha Processor package
//
// Author:  Tim Stark
// Date:    May 14, 2023

#pragma once

#define AXP_NGREGS  32      // 32 general registers
#define AXP_NFREGS  32      // 32 floating-point registers

// Implementation Version
#define ARCH_EV4   0
#define ARCH_EV5   1
#define ARCH_EV6   2
#define ARCH_EV7   3

// Architectural Feature Bits
#define ARCH_PFM   0x1000
#define ARCH_PRC   0x0200
#define ARCH_MVI   0x0100 // Multimedia Extension
#define ARCH_CIX   0x0004 // Count Extension
#define ARCH_FIX   0x0002 // SQR/FP Convert Extension
#define ARCH_BWX   0x0001 // Byte/Word Extensions

// Exceptions
//#define EXC_RSVIN   0x01  // Reserved Instruction
//#define EXC_RSVOPR  0x02  // Reserved Operand
//#define EXC_ALIGN   0x03  // Operand Alignment
//#define EXC_FPDIS   0x04  // Floating-Point Disable
//#define EXC_TBM     0x08  // Translation Buffer Miss
//#define EXC_FOX     0x10  // Fault on Read/Write/Execute
//#define EXC_ACV     0x14  // Access Control Violation
//#define EXC_TNV     0x18  // Translation Not Valid
//#define EXC_BVA     0x1C  // Bad Virtual Address
//#define EXC_EXECUTE 0x00  // Offset for Execute
//#define EXC_READ    0x01  // Offset for Read
//#define EXC_WRITE   0x02  // Offset for Write

// Internal exception codes for all processors
#define EXC_OPCODE    1    // Illegal opcode
#define EXC_ITBMISS   2    // Istream TB miss
#define EXC_DTBMISS1  3    // Dstream TB miss single
#define EXC_DTBMISS2  4    // Dstream TB miss double w/VPTE
#define EXC_IACV      5    // Istream access violation
#define EXC_FPDIS     6    // Floating-point attempted operation

// Traps
#define TRAP_SWC     0x01 // Software Completion
#define TRAP_INV     0x02 // Invalid Operand
#define TRAP_DZE     0x04 // Divide By Zero
#define TRAP_OVF     0x08 // Overflow
#define TRAP_UNF     0x10 // Underflow
#define TRAP_INE     0x20 // Inexact
#define TRAP_IOV     0x40 // Integer Overflow
#define TRAP_SUMM_RW 0x7F // Trap Summary Mask

// AXP Processor Instruction Field
//
// PALcode Format
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |                     Number                        |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//
// Branch Format
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |    RA   |              Displacement               |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//
// Memory Format
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |    RA   |    RB   |          Displacement         |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//
// Memory Format with function code
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |    RA   |    RB   |           Function            |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//
// Operate Format
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |    RA   |    RB   | SBZ |0|   Function  |    RC   |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |    RA   |       LIT     |1|   Function  |    RC   |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
// Floating-point Operate Format
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |    RA   |    RB   |       Function      |    RC   |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0

// Instruction Flags
// Opcode Modes
#define CLS_GRP    0    // Instructions w/Function Codes
#define CLS_PAL    1    // PALcode Instruction
#define CLS_BRA    2    // Branch Instruction
#define CLS_MEM    3    // Memory Instruction
#define CLS_IOP    4    // Operate Instruction (Integer)
#define CLS_FOP    5    // Floating-Point Instruction

// PALcode Modes
#define PAL_NTYPES 4   // Number of PALcode Types
#define PAL_NCODES 256 // Number of PALcode Functions
#define PAL_RAW    0   // PALcodes for Raw Hardware
#define PAL_VMS    1   // PALcodes for OpenVMS
#define PAL_UNX    2   // PALcodes for Digital UNIX
#define PAL_WNT    3   // PALcodes for Windows NT

// Operand Flags
#define OPR_FUNC   0x80000000  // Function Code
#define OPR_TYPE   0x40000000  // PALcode Type
#define OPR_WNT    0x20000000  //    Windows NT
#define OPR_UNX    0x10000000  //    Digital UNIX
#define OPR_VMS    0x08000000  //    OpenVMS
#define OPR_IPR    0x00040000  // IPR Number
#define OPR_PAL    0x00020000  // PALcode Number
#define OPR_MJP    0x00010000  // Jump Displacement
#define OPR_MDP    0x00008000  // Memory Displacement
#define OPR_BDP    0x00004000  // Branch Displacement
#define OPR_IDX    0x00002000  // Register B - Index
#define OPR_LIT    0x00001000  // Register B - 8-bit Literal
#define OPR_RC     0x00000400  // Register C
#define OPR_RB     0x00000200  // Register B
#define OPR_RA     0x00000100  // Register A
#define OPR_NONE   0x00000000  // No Operands (Hardware PALcode)
#define OPR_RBL    (OPR_RB|OPR_LIT)

// Instruction Field Defintions
#define OPP_CODE   26        // Operation Code
#define OPP_JUMP   14        // Jump Code
#define OPP_FUNC    5        // Function Code
#define OPP_LIT    13        // Literal Immediate
#define OPP_RA     21        // Register A
#define OPP_RB     16        // Register B
#define OPP_RC      0        // Register C
#define OPC_LIT   (1u << 12) // Literate Bit
#define OPC_MASK  0x3F       // Opcode Mask
#define FNC_MASK  0x7FF      // Function Mask

#define OP_GETOP(opc)  (((opc) >> OPP_CODE) & OPC_MASK)
#define OP_GETJMP(opc) (((opc) >> OPP_JUMP) & 0x3)
#define OP_GETFNC(opc) (((opc) >> OPP_FUNC) & FNC_MASK)
#define OP_GETRA(opc)  (((opc) >> OPP_RA) & REG_MASK)
#define OP_GETRB(opc)  (((opc) >> OPP_RB) & REG_MASK)
#define OP_GETRC(opc)  (((opc) >> OPP_RC) & REG_MASK)
#define OP_GETLIT(opc) (((opc) >> OPP_LIT) & 0xFF)
#define OP_GETPAL(opc) ((opc) & 0x03FFFFFF)
#define OP_GETBDP(opc) SXT21((opc) & 0x001FFFFF)
#define OP_GETMDP(opc) ((opc) & 0xFFFF)
#define OP_GETMEM(opc) ((opc) & 0xFFFF)
#define OP_GETMJP(opc) ((opc) & 0x3FFF)

// Access Mode
#define ACC_KERNEL		0	// Kernel
#define ACC_EXECUTIVE	1	// Executive
#define ACC_SUPERVISOR	2	// Supervisor
#define ACC_USER		3	// User

// Read/write access flags
#define ACC_READ		0	// read access
#define ACC_WRITE		1	// write access
#define ACC_EXEC		2	// execute access

#define ACC_MODE		0x0003	 // Access mode mask
#define ACC_ALTCM		0x0004   // Alternate current mode flag
#define ACC_VPTE        0x0008   // Virtual page table entry

// Memory Management - Superpage definitions
// #define SPE0_MASK       0x0000'FFFF'C000'0000ull
// #define SPE0_MATCH      0x0000'FFFF'8000'0000ull
// #define SPE0_MAP        0x0000'0000'3FFF'FFFFull

// #define SPE1_MASK       0x0000'FE00'0000'0000ull
// #define SPE1_MATCH      0x0000'FC00'0000'0000ull
// #define SPE1_MAP        0x0000'01FF'FFFF'FFFFull
// #define SPE1_TEST       0x0000'0100'0000'0000ull
// #define SPE1_ADD        0x0000'0E00'0000'0000ull

// #define SPE2_MASK       0x0000'C000'0000'0000ull
// #define SPE2_MATCH      0x0000'8000'0000'0000ull
// #define SPE2_MAP        0x0000'0FFF'FFFF'FFFFull


#define ABORT(why)

#define FPSTART if (state.fpen == 0) ABORT(EXC_FPDIS);

// PC field definition
#define PC_PAL_MODE		1	// PC PAL mode field

// Register definitions
#define REG_SIZE	32
#define REG_MASK	(REG_SIZE - 1)
#define REG_ZERO	(REG_SIZE - 1)
#define REG_SP		(REG_SIZE - 2)
#define RREG(reg)	((reg) & REG_MASK)
//#define RREG(reg)	((reg) & REG_MASK) + (state.vpcReg & PC_PAL_MODE) && ((reg) & 0x0c) == 0x04) && state.sde ? (REG_MASK+1) : 0)

#define RREG2(reg) (((reg) & REG_MASK) + \
	(((axp.vpcAddr & 1) && (((reg) & 0x0C) == 0x04) && axp.sde) ? (REG_MASK+1) : 0))

// executing instruction definitions
#define RA		RREG2(OP_GETRA(axp.opWord))
#define RB		RREG2(OP_GETRB(axp.opWord))
#define RC		RREG2(OP_GETRC(axp.opWord))
#define RAV		axp.gRegs[RA]
#define RBV		axp.gRegs[RB]
#define RBVL	((axp.opWord & OPC_LIT) ? OP_GETLIT(axp.opWord) : axp.gRegs[RB])
#define RCV		axp.gRegs[RC]

#define FA		OP_GETRA(axp.opWord)
#define FB		OP_GETRB(axp.opWord)
#define FC		OP_GETRC(axp.opWord)
#define FAV		state.fRegs[RA]
#define FBV		state.fRegs[RB]
#define FCV		state.fRegs[RC]

#define DISP12  SXT12(axp.opWord)
#define DISP16	OP_GETMDP(axp.opWord)
#define DISP21  SXT21(axp.opWord)

#define SXT12(val) SXTL((int32_t)(((val) & 0x800) ? ((val) | 0xFFFFF000) : ((val) & 0x00000FFF)))
#define SXT21(val) SXTL((int32_t)(((val) & 0x100000) ? ((val) | 0xFFE00000) : ((val) & 0x001FFFFF)))

#define SXT48(val) (((val) & 0x0000800000000000LL) \
	? ((val) | 0xFFFF000000000000LL) : ((val) & 0x0000FFFFFFFFFFFFLL))

#define setPC(vAddr)    { axp.vpcAddr = vAddr; }
#define addPC(disp)     { axp.vpcAddr += disp; }
#define nextPC()        { axp.vpcAddr += 4; }

#include "emu/map/map.h"

class AlphaProcessor : public CPUDevice
{
public:
    AlphaProcessor(SystemConfig &config, cDeviceType &type,
        cstr_t &devName, Device *owner, uint64_t clock,
        int paWidth, int aWidth);
    ~AlphaProcessor() = default;

    map::AddressConfigList getAddressConfigList() const;
    
protected:
    struct axpState
    {
        uint64_t gRegs[AXP_NGREGS*2];   // 64-bit general registers (with shadow registers)
        uint64_t fRegs[AXP_NFREGS];     // 64-bit floating registers

        uint64_t vpcAddr;               // Virtual PC address
        uint64_t fpcAddr;               // Faulting PC address

        uint64_t excAddr;               // Exceptions
        uint64_t palBase;               // PAL Base Address

        uint32_t opWord;                // Current instruction word

        bool sde = false;               // Shadow enable flag

        int icm = ACC_KERNEL;           // Current access mode
        int dcm;                        // DTB access mode
        int altcm;                      // Alternative access mode

    } axp;

    map::AddressConfig mapProgramConfig;

    map::MemoryAccess<64, 3, 0, LittleEndian>::specific mapProgram;

    // void translate();

    // Virtual Hardware instructions for specific processors
    virtual void pal_call() = 0;
    virtual void hw_ld() = 0;
    virtual void hw_st() = 0;
    virtual void hw_rei() = 0;
    virtual void hw_mtpr() = 0;
    virtual void hw_mfpr() = 0;

};