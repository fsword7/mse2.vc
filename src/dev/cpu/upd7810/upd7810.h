// upd7810.h - NEC uPD7810 processor package
//
// Author:  Tim Stark
// Date:    Jan 9, 2022

#pragma once

#include "emu/devcpu.h"

struct upd7810_opCode_t
{
    cchar_t  *opName;
    cchar_t  *opLine;
    uint8_t   opField;
    uint8_t   opCode;
    uint8_t   opMask;
};

class upd7810_cpuDevice : public ProcessorDevice
{
protected:
    upd7810_cpuDevice(const SystemConfig &config, const DeviceType &type,
        cstag_t &devName, Device *owner, uint64_t clock);

public:    
    enum
    {
        Prefix = 0, Illegal,
        ACI,    ADC,    ADCW,   ADCX,   ADD,    ADDNC,  ADDNCW, ADDNCX,
        ADDW,   ADDX,   ADI,    ADINC,  ANA,    ANAW,   ANAX,   AND,
        ANI,    ANIW,   BIT,    BLOCK,  CALB,   CALF,   CALL,   CALT,
        CLC,    CLR,    CMC,    DAA,    DADC,   DADD,   DADDNC, DAN,
        DCR,    DCRW,   DCRX,   DCX,    DEQ,    DGT,    DI,     DIV,    
        DLT,    DMOV,   DNE,    DOFF,   DON,    DOR,    DRLL,   DRLR,
        DSBB,   DSLL,   DSLR,   DSUB,   DSUBNB, DXR,    EADD,   EI,
        EQA,    EQAW,   EQAX,   EQI,    EQIW,   ESUB,   EX,     EXA,
        EXH,    EXX,    EXR,    GTA,    GTAW,   GTAX,   GTI,    GTIW,
        HALT,   IN,     INR,    INRW,   INX,    JB,     JEA,    JMP,
        JR,     JRE,    LBCD,   LDAW,   LDAX,   LDEAX,  LDED,   LHLD,
        LSPD,   LTA,    LTAW,   LTAX,   LTI,    LTIW,   LXI,    MOV,
        MUL,    MVI,    MVIW,   MVIX,   NEA,    NEAW,   NEAX,   NEGA,
        NEI,    NEIW,   NOP,    NOT,    OFFA,   OFFAW,  OFFAX,  OFFI,
        OFFIW,  ONA,    ONAW,   ONAX,   ONI,    ONIW,   OR,     ORA,
        ORAW,   ORAX,   ORI,    ORIW,   OUT,    PER,    PEX,    POP,
        PUSH,   RET,    RETI,   RETS,   RLD,    RLL,    RLR,    RRD,
        SBB,    SBBW,   SBBX,   SBCD,   SBI,    SDED,   SETB,   SHLD,
        SID,    SK,     SKIT,   SKN,    SKNIT,  SLL,    SLLC,   SLR,
        SLRC,   SOFTI,  SSPD,   STAW,   STAX,   STC,    STEAX,  STM,
        STOP,   SUB,    SUBNB,  SUBNBW, SUBNBX, SUBW,   SUBX,   SUI,
        SUINB,  TABLE,  XOR,    XRA,    XRAW,   XRAX,   XRI  
    };

    struct dopTable_t
    {
        dopTable_t()
        : token(Illegal), operand(nullptr)
        { }

        dopTable_t(uint8_t tok, cchar_t *opr)
        : token(tok), operand(opr) 
        { }

        dopTable_t(const dopTable_t (&table)[256])
        : token(Prefix), operand(table)
        { }

        uint8_t     token;
        const void *operand;

        bool isPrefix() const { return token == Prefix; }

        const dopTable_t *getPrefix() const
        {
            assert (token == Prefix);
            return reinterpret_cast<const dopTable_t *>(operand);
        }

        cchar_t *getOperand() const
        {
            return (token != Prefix) ? reinterpret_cast<cchar_t *>(operand) : nullptr;
        }
    };

    upd7810_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
    virtual ~upd7810_cpuDevice() = default;

    map::AddressConfigList getAddressConfigList() const;

    void devStart() override;

    uint64_t executeClockToCycle(uint64_t clocks) const override { return (clocks + 3 - 1) / 3; }
    uint64_t executeCycleToClock(uint64_t cycles) const override { return (cycles * 3); }
    uint64_t executeGetMinCycles() const override { return 1; }
    uint64_t executeGetMaxCycles() const override { return 40; }
    // int executeGetInputLines()     { return 2; }

    int list(offs_t vAddr) override;

protected:
    void setData128(map::AddressList &map);
    void setData256(map::AddressList &map);
    // void setROM4096(map::AddressList &map);
 
    map::AddressConfig mapProgramConfig;

    map::MemoryAccess<16, 0, 0, LittleEndian>::specific mapProgram;

    int64_t cpuCycles = 0;

    typedef void (upd7810_cpuDevice::*opFunc_t)();

    struct opCode_t
    {
        opFunc_t opFunc;
        uint8_t  opLength;
        uint8_t  opUseCycles;
        uint8_t  opSkipCycles;
        uint8_t  opMask;
    };

    // Opcode table for execution
    const opCode_t *opXX = nullptr;
    const opCode_t *op48 = nullptr;
    const opCode_t *op4C = nullptr;
    const opCode_t *op4D = nullptr;
    const opCode_t *op60 = nullptr;
    const opCode_t *op64 = nullptr;
    const opCode_t *op70 = nullptr;
    const opCode_t *op74 = nullptr;

    const dopTable_t *dopCode = dopXX_7810;

    static const dopTable_t dopXX_7810[256];
    static const dopTable_t dop48_7810[256];
    static const dopTable_t dop4C_7810[256];
    static const dopTable_t dop4D_7810[256];
    static const dopTable_t dop60_78XX[256];
    static const dopTable_t dop64_7810[256];
    static const dopTable_t dop70_78XX[256];
    static const dopTable_t dop74_78XX[256]; 
};

DECLARE_DEVICE_TYPE(UPD7810, upd7810_cpuDevice);