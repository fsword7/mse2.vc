// mcs80exec.cpp - 8080/8080A/8085 processor - execute routines
//
// Author:  Tim Stark
// Date:    Dec 23, 2021

#include "emu/core.h"
#include "emu/map/map.h"
#include "dev/cpu/mcs80/mcs80.h"
#include "dev/cpu/mcs80/mcs80op.h"
#include "dev/cpu/mcs80/mcs80dbg.h"

const uint8_t i8080_cpuCycles[256] =
{
    4, 10,  7,  5,  5,  5,  7,  4,  4, 10,  7,  5,  5,  5,  7,  4, // 00-0F
    4, 10,  7,  5,  5,  5,  7,  4,  4, 10,  7,  5,  5,  5,  7,  4, // 10-1F
    4, 10, 16,  5,  5,  5,  7,  4,  4, 10, 16,  5,  5,  5,  7,  4, // 20-2F
    4, 10, 13,  5, 10, 10, 10,  4,  4, 10, 13,  5,  5,  5,  7,  4, // 30-3F
    5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5, // 40-4F 
    5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5, // 50-5F 
    5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5, // 60-6F 
    7,  7,  7,  7,  7,  7,  7,  7,  5,  5,  5,  5,  5,  5,  7,  5, // 70-7F 
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // 80-8F
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // 90-9F
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // A0-AF
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // B0-BF
    5, 10, 10, 10, 11, 11,  7, 11,  5, 10, 10, 10, 11, 11,  7, 11, // C0-CF
    5, 10, 10, 10, 11, 11,  7, 11,  5, 10, 10, 10, 11, 11,  7, 11, // D0-DF
    5, 10, 10, 10, 11, 11,  7, 11,  5,  5, 10,  5, 11, 11,  7, 11, // E0-EF
    5, 10, 10,  4, 11, 11,  7, 11,  5,  5, 10,  4, 11, 11,  7, 11  // F0-FF
};

const uint8_t i8085_cpuCycles[256] =
{
    4, 10,  7,  6,  4,  4,  7,  4, 10, 10,  7,  6,  4,  4,  7,  4, // 00-0F
    7, 10,  7,  6,  4,  4,  7,  4, 10, 10,  7,  6,  4,  4,  7,  4, // 10-1F
    7, 10, 16,  6,  4,  4,  7,  4, 10, 10, 16,  6,  4,  4,  7,  4, // 20-2F
    7, 10, 13,  6, 10, 10, 10,  4, 10, 10, 13,  6,  4,  4,  7,  4, // 30-3F
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // 40-4F 
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // 50-5F 
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // 60-6F 
    7,  7,  7,  7,  7,  7,  5,  7,  4,  4,  4,  4,  4,  4,  7,  4, // 70-7F 
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // 80-8F
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // 90-9F
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // A0-AF
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // B0-BF
    6, 10, 10, 10, 11, 12,  7, 12,  6, 10, 10, 12, 11, 11,  7, 12, // C0-CF
    6, 10, 10, 10, 11, 12,  7, 12,  6, 10, 10, 10, 11, 10,  7, 12, // D0-DF
    6, 10, 10, 16, 11, 12,  7, 12,  6,  6, 10,  5, 11, 10,  7, 12, // E0-EF
    6, 10, 10,  4, 11, 12,  7, 12,  6,  6, 10,  4, 11, 10,  7, 12  // F0-FF
};

void mcs80_cpuDevice::init()
{

    for (int val = 0; val < 256; val++)
    {
        zspFlags[val] = 0;

        int p = 0;
        for (int bit = 0; bit < 8; bit++)
            if (val & (1u << bit))
                p++;
        if (p & 1)
            zspFlags[val] |= PSW_PF;

        if (val == 0)
            zspFlags[val] |= PSW_ZF;
    }

    opCycleTable = is8080() ? i8080_cpuCycles : i8085_cpuCycles;
}

void mcs80_cpuDevice::reset()
{
    REG_PC = 0;
}

uint8_t mcs80_cpuDevice::readi8()
{
    return mapProgram.read8(REG_PC++);
}

uint16_t mcs80_cpuDevice::readi16()
{
    return mapProgram.read8(REG_PC++) |
          (mapProgram.read8(REG_PC++) << 8);
}

void mcs80_cpuDevice::enableInterrupts(bool enable)
{

}

uint8_t mcs80_cpuDevice::read8(offs_t addr)
{
    return mapProgram.read8(addr);
}

void mcs80_cpuDevice::write8(offs_t addr, uint8_t data)
{
    mapProgram.write8(addr, data);
}

void mcs80_cpuDevice::opPUSH(uint16_t val)
{
    mapProgram.write8(--REG_SP, val >> 8);
    mapProgram.write8(--REG_SP, val);
}

uint16_t mcs80_cpuDevice::opPOP()
{
    return mapProgram.read8(REG_SP++) |
          (mapProgram.read8(REG_SP++) << 8);
}

uint8_t mcs80_cpuDevice::opINR(uint8_t val)
{
    uint8_t hc = ((val & 0x0F) == 0x0F) ? PSW_HF : 0;
    REG_F = (REG_F & PSW_CF) | zspFlags[++val] | hc;
    return val;
}

uint8_t mcs80_cpuDevice::opDCR(uint8_t val)
{
    uint8_t hc = ((val & 0x0F) == 0x0F) ? PSW_HF : 0;
    REG_F = (REG_F & PSW_CF) | zspFlags[--val] | hc | PSW_VF;
    return val;
}

void mcs80_cpuDevice::opDAD(uint16_t val)
{
    int add = REG_HL + val;
    REG_F = (REG_F & ~PSW_CF) | ((add >> 16) & PSW_CF);
    REG_HL = add;
}

void mcs80_cpuDevice::opADD(uint8_t val)
{
    int add = REG_A + val;
    REG_F = zspFlags[add & 0xFF] | ((add >> 8) & PSW_CF) |
        (~(REG_A ^ add ^ val) & PSW_CF) | PSW_VF;
    REG_A = add;
}

void mcs80_cpuDevice::opADC(uint8_t val)
{
    int add = REG_A + val + (REG_F & PSW_CF);
    REG_F = zspFlags[add & 0xFF] | ((add >> 8) & PSW_CF) |
        (~(REG_A ^ add ^ val) & PSW_CF) | PSW_VF;
    REG_A = add;
}

void mcs80_cpuDevice::opSUB(uint8_t val)
{
    int sub = REG_A - val;
    REG_F = zspFlags[sub & 0xFF] | ((sub >> 8) & PSW_CF) |
        (~(REG_A ^ sub ^ val) & PSW_HF) | PSW_VF;
    REG_A = sub;
}

void mcs80_cpuDevice::opSBB(uint8_t val)
{
    int sub = REG_A - val - (REG_F & PSW_CF);
    REG_F = zspFlags[sub & 0xFF] | ((sub >> 8) & PSW_CF) |
        (~(REG_A ^ sub ^ val) & PSW_HF) | PSW_VF;
    REG_A = sub;
}

void mcs80_cpuDevice::opCMP(uint8_t val)
{
    int diff = REG_A - val;
    REG_F = zspFlags[diff & 0xFF] | ((diff >> 8) & PSW_CF) |
        (~(REG_A ^ diff ^ val) & PSW_HF) | PSW_VF;
}

void mcs80_cpuDevice::opANA(uint8_t val)
{
    uint8_t hc = ((REG_A | val) << 1) & PSW_HF;
    REG_A &= val;
    REG_F = zspFlags[REG_A];
    REG_F |= is8085() ? PSW_HF : hc;
}

void mcs80_cpuDevice::opORA(uint8_t val)
{
    REG_A |= val;
    REG_F = zspFlags[REG_A];
}

void mcs80_cpuDevice::opXRA(uint8_t val)
{
    REG_A ^= val;
    REG_F = zspFlags[REG_A];
}


void mcs80_cpuDevice::opCALL(bool flag)
{
    uint16_t addr = readi16();
    if (flag == true)
    {
        opPUSH(REG_PC);
        REG_PC = addr;
    }
}

void mcs80_cpuDevice::opRET(bool flag)
{
    if (flag == true)
        REG_PC = opPOP();
}

void mcs80_cpuDevice::opJMP(bool flag)
{
    uint16_t addr = readi16();
    if (flag == true)
        REG_PC = addr;
}

void mcs80_cpuDevice::opRST(uint8_t val)
{
    opPUSH(REG_PC);
    REG_PC = val * 8;
}

void mcs80_cpuDevice::step()
{
    // Display disassembly line
    list(REG_PC);

    // Execute one instruction.
    opCount = 0;
    executeRun();

    // Display results
    fmt::printf("A=%02X F=%02X B=%02X C=%02X D=%02X E=%02X H=%02X L=%02X\n",
        REG_A, REG_F, REG_B, REG_C, REG_D, REG_E, REG_H, REG_L);
    fmt::printf("AF=%04X BC=%04X DE=%04X HL=%04X SP=%04X PC=%04X\n",
        REG_AF, REG_BC, REG_DE, REG_HL, REG_SP, REG_PC);
}

void mcs80_cpuDevice::executeRun()
{

    do
    {
        execute();
    }
    while (opCount > 0);
}

void mcs80_cpuDevice::execute()
{
    uint8_t opCode;
    uint16_t tval;
    int cFlag;
    
    pcBase = REG_PC;
    opCode = readi8();

    // Takes CPU cycles each instruction
    opCount =- opCycleTable[opCode];

    switch (opCode)
    {
        case 0x00:
            // NOP instruction
            OP_EXEC(NOP, NOP, IMPL);
            break;

        case 0x76:
            // HLT instruction
            OP_EXEC(HLT, HLT, IMPL);
            break;

        case 0xF3:
            // EI instruction
            OP_EXEC(EI, EI, IMPL);
            break;

        case 0xFB:
            // DI instruction
            OP_EXEC(DI, DI, IMPL);
            break;

        case 0x37:
            // STC instruction
            OP_EXEC(STC, STC, IMPL);
            break;

        case 0x3F:
            // CMC instruction
            OP_EXEC(CMC, CMC, IMPL);
            break;

        case 0x2F:
            // CMA instruction
            OP_EXEC(CMA, CMA, IMPL);
            break;

        case 0xC7: case 0xCF: case 0xD7: case 0xDF:
        case 0xE7: case 0xEF: case 0xF7: case 0xFF:
            // RST n instruction
            OP_EXEC(RST, RST, RESET);
            break;

        case 0x0A: case 0x1A:
            // LDAX r insruction
            OP_EXEC(LDAX, LDAX, REGWSP);
            break;
        case 0x2A:
            // LHLD adr instruction
            tval = readi16();
            OP_EXEC(LHLD, LHLD, ADR);
            break;
        case 0x3A:
            // LDA adr instruction
            tval = readi16();
            OP_EXEC(LDA, LDA, ADR);
            break;

        case 0x01: case 0x11: case 0x21: case 0x31:
            // LXI r,d16 instruction
            tval = readi16();
            OP_EXEC(LXI, LXI, REGWSPI);
            break;

        case 0x02: case 0x12:
            // STAX r instruction
            OP_EXEC(STAX, STAX, REGWSP);
            break;
        case 0x22:
            // SHLD adr instruction
            tval = readi16();
            OP_EXEC(SHLD, SHLD, ADR);
            break;
        case 0x32:
            // STA adr instruction
            tval = readi16();
            OP_EXEC(STA, STA, ADR);
            break;

        case 0x40: case 0x41: case 0x42: case 0x43:
        case 0x44: case 0x45:            case 0x47:
        case 0x48: case 0x49: case 0x4A: case 0x4B:
        case 0x4C: case 0x4D:            case 0x4F:
        case 0x50: case 0x51: case 0x52: case 0x53:
        case 0x54: case 0x55:            case 0x57:
        case 0x58: case 0x59: case 0x5A: case 0x5B:
        case 0x5C: case 0x5D:            case 0x5F:
        case 0x60: case 0x61: case 0x62: case 0x63:
        case 0x64: case 0x65:            case 0x67:
        case 0x68: case 0x69: case 0x6A: case 0x6B:
        case 0x6C: case 0x6D:            case 0x6F:
        case 0x78: case 0x79: case 0x7A: case 0x7B:
        case 0x7C: case 0x7D:            case 0x7F:
            // MOV r,r instruction
            OP_EXEC(MOV, MOV, MOVE);
            break;

        case 0x46: case 0x4E: case 0x56: case 0x5E:
        case 0x66: case 0x6E:            case 0x7E:
            // MOV r,M instruction
            OP_EXEC(MOV, MOVrM, MOVErM);
            break;

        case 0x70: case 0x71: case 0x72: case 0x73:
        case 0x74: case 0x75:            case 0x77:
            // MOV M,r instruction
            OP_EXEC(MOV, MOVMr, MOVEMr);
            break;

        case 0x06: case 0x0E: case 0x16: case 0x1E:
        case 0x26: case 0x2E:            case 0x3E:
            // MVI r,d8 instruction
            tval = readi8();
            OP_EXEC(MVI, MVI, REGI8);
            break;
        case 0x36:
            // MVI M,d8, instruction
            tval = readi8();
            OP_EXEC(MVI, MVIM, REGMI8);
            break;

        case 0x04: case 0x0C: case 0x14: case 0x1C:
        case 0x24: case 0x2C:            case 0x3C:
            // INR r instruction
            OP_EXEC(INR, INR, REG);
            break;
        case 0x34:
            OP_EXEC(INR, INRM, REGM);
            break;

        case 0x05: case 0x0D: case 0x15: case 0x1D:
        case 0x25: case 0x2D:            case 0x3D:
            // DCR r instruction
            OP_EXEC(DCR, DCR, REG);
            break;
        case 0x35:
            OP_EXEC(DCR, DCRM, REGM);
            break;

        case 0x03: case 0x13: case 0x23: case 0x33:
            // INX r instruction
            OP_EXEC(INX, INX, REGWSP);
            break;

        case 0x0B: case 0x1B: case 0x2B: case 0x3B:
            // DCX r instruction
            OP_EXEC(DCX, DCX, REGWSP);
            break;

        case 0x07:
            // RLC instruction
            OP_EXEC(RLC, RLC, IMPL);
            break;
        case 0x0F:
            // RRC instruction
            OP_EXEC(RRC, RRC, IMPL);
            break;
        case 0x17:
            // RAL instruction
            OP_EXEC(RAL, RAL, IMPL);
            break;
        case 0x1F:
            // RAR instruction
            OP_EXEC(RAR, RAR, IMPL);
            break;

        case 0x09: case 0x19: case 0x29: case 0x39:
            // DAD r instruction
            OP_EXEC(DAD, DAD, REGWSP);
            break;

        case 0x80: case 0x81: case 0x82: case 0x83:
        case 0x84: case 0x85:            case 0x87:
            // ADD r instruction
            OP_EXEC(ADD, ADD, REG);
            break;
        case 0x86:
            // ADD M instruction
            OP_EXEC(ADD, ADDM, REG);
            break;
        case 0xC6:
            // ADI d8 instruction
            tval = readi8();
            OP_EXEC(ADI, ADI, IMM8);
            break;

        case 0x88: case 0x89: case 0x8A: case 0x8B:
        case 0x8C: case 0x8D:            case 0x8F:
            // ADC r instruction
            OP_EXEC(ADC, ADC, REG);
            break;
        case 0x8E:
            // ADC M instruction
            OP_EXEC(ADC, ADCM, REG);
            break;
        case 0xCE:
            // ACI d8 instruction
            tval = readi8();
            OP_EXEC(ACI, ACI, IMM8);
            break;

        case 0x90: case 0x91: case 0x92: case 0x93:
        case 0x94: case 0x95:            case 0x97:
            // SUB r instruction
            OP_EXEC(SUB, SUB, REG);
            break;
        case 0x96:
            // SUB M instruction
            OP_EXEC(SUB, SUBM, REG);
            break;
        case 0xD6:
            // SUI d8 instruction
            tval = readi8();
            OP_EXEC(SUI, SUI, IMM8);
            break;

        case 0x98: case 0x99: case 0x9A: case 0x9B:
        case 0x9C: case 0x9D:            case 0x9F:
            // SBB r instruction
            OP_EXEC(SBB, SBB, REG);
            break;
        case 0x9E:
            // SBB M instruction
            OP_EXEC(SBB, SBBM, REG);
            break;
        case 0xDE:
            // SBI d8 instruction
            tval = readi8();
            OP_EXEC(SBI, SBI, IMM8);
            break;

        case 0xA0: case 0xA1: case 0xA2: case 0xA3:
        case 0xA4: case 0xA5:            case 0xA7:
            // ANA r instruction
            OP_EXEC(ANA, ANA, REG);
            break;
        case 0xA6:
            // ANA M instruction
            OP_EXEC(ANA, ANAM, REG);
            break;
        case 0xE6:
            // ANI d8 instruction
            tval = readi8();
            OP_EXEC(ANI, ANI, IMM8);
            break;

        case 0xA8: case 0xA9: case 0xAA: case 0xAB:
        case 0xAC: case 0xAD:            case 0xAF:
            // XRA r instruction
            OP_EXEC(XRA, XRA, REG);
            break;
        case 0xAE:
            // XRA M instruction
            OP_EXEC(XRA, XRAM, REG);
            break;
        case 0xEE:
            // XRI d8 instruction
            tval = readi8();
            OP_EXEC(XRI, XRI, IMM8);
            break;
            
        case 0xB0: case 0xB1: case 0xB2: case 0xB3:
        case 0xB4: case 0xB5:            case 0xB7:
            // ORA r instruction
            OP_EXEC(ORA, ORA, REG);
            break;
        case 0xB6:
            // ORA M instruction
            OP_EXEC(ORA, ORAM, REG);
            break;
        case 0xF6:
            // ORI d8 instruction
            tval = readi8();
            OP_EXEC(ORI, ORI, IMM8);
            break;

        case 0xB8: case 0xB9: case 0xBA: case 0xBB:
        case 0xBC: case 0xBD:            case 0xBF:
            // CMP r instruction
            OP_EXEC(CMP, CMP, REG);
            break;
        case 0xBE:
            // CMP M instruction
            OP_EXEC(CMP, CMPM, REG);
            break;
        case 0xFE:
            // CPI d8 instruction
            tval = readi8();
            OP_EXEC(CPI, CPI, IMM8);
            break;

        case 0xC4: // CNZ nnnn instruction
            opCALL((REG_F & PSW_ZF) == 0);
            break;
        case 0xCC: // CZ nnnn instruction
            opCALL(REG_F & PSW_ZF);
            break;
        case 0xD4: // CNC nnnn
            opCALL((REG_F & PSW_CF) == 0);
            break;
        case 0xDC: // CC nnnn
            opCALL(REG_F & PSW_CF);
            break;
        case 0xE4: // CPO nnnn instruction
            opCALL((REG_F & PSW_PF) == 0);
            break;
        case 0xEC: // CPE nnnn instruction
            opCALL(REG_F & PSW_PF);
            break;
        case 0xF4: // CP nnnn instruction
            opCALL(REG_F & PSW_SF);
            break;
        case 0xFC: // CM nnnn instruction
            opCALL(REG_F & PSW_SF);
            break;

        case 0xCD: // CALL nnnn instruction
            opCALL(true);
            break;


        case 0xC0: // RNZ instruction
            opRET((REG_F & PSW_ZF) == 0);
            break;
        case 0xC8: // RZ instruction
            opRET(REG_F & PSW_ZF);
            break;
        case 0xD0: // RNC instruction
            opRET((REG_F & PSW_CF) == 0);
            break;
        case 0xD8: // RC instruction
            opRET(REG_F & PSW_CF);
            break;
        case 0xE0: // RPO instruction
            opRET((REG_F & PSW_PF) == 0);
            break;
        case 0xE8: // RPE instruction
            opRET(REG_F & PSW_PF);
            break;
        case 0xF0: // RP instruction
            opRET(REG_F & PSW_SF);
            break;
        case 0xF8: // RM instruction
            opRET(REG_F & PSW_SF);
            break;

        case 0xC9: // RET instruction
            REG_PC = opPOP();
            break;


        case 0xC2: // JNZ nnnn instruction
            opJMP((REG_F & PSW_ZF) == 0);
            break;
        case 0xCA: // JZ instruction
            opJMP(REG_F & PSW_ZF);
            break;
        case 0xD2: // JNC nnnn
            opJMP((REG_F & PSW_CF) == 0);
            break;
        case 0xDA: // JC nnnn
            opJMP(REG_F & PSW_CF);
            break;
        case 0xE2: // JPO nnnn instruction
            opJMP((REG_F & PSW_PF) == 0);
            break;
        case 0xEA: // JPE instruction
            opJMP(REG_F & PSW_PF);
            break;
        case 0xF2: // JP nnnn instruction
            opJMP(REG_F & PSW_SF);
            break;
        case 0xFA: // JM nnnn instruction
            opJMP(REG_F & PSW_SF);
            break;

        case 0xC3: // JMP nnnn instruction
            opJMP(true);
            break;

        case 0xDD: // JNX nnnn instruction (8085 only)
            if (is8085())
                opJMP((REG_F & PSW_X5F) == 0);
            else
                opCALL(true);
            break;
        case 0xFD: // JX nnnn instruction (8085 only)
            if (is8085())
                opJMP(REG_F & PSW_X5F);
            else
                opCALL(true);
            break;


        case 0xC5: // PUSH B instruction
            opPUSH(REG_BC);
            break;
        case 0xD5: // PUSH D
            opPUSH(REG_DE);
            break;
        case 0xE5: // PUSH H instruction
            opPUSH(REG_HL);
            break;
        case 0xF5: // PUSH A instruction
            if (is8080())
                REG_F = (REG_F & ~(PSW_X3F|PSW_X5F)) | PSW_VF;
            opPUSH(REG_AF);
            break;

        case 0xC1: // POP B instruction
            REG_BC = opPOP();
            break;
        case 0xD1: // POP D
            REG_DE = opPOP();
            break;
        case 0xE1: // POP H instruction
            REG_HL = opPOP();
            break;
        case 0xF1: // POP A instruction
            REG_AF = opPOP();
            break;

        case 0xDB: // IN nn
            tval = readi8();
            REG_A = mapIOPort.read8(tval);
            break;
        case 0xD3: // OUT nn
            tval = readi8();
            mapIOPort.write8(tval, REG_A);
            break;

        case 0xE9: // PCHL instruction
            REG_PC = REG_HL;
            break;
        case 0xF9: // SPHL instruction
            REG_SP = REG_HL;
            break;

        case 0xE3: // XTHL instruction
            tval = opPOP();
            opPUSH(REG_HL);
            REG_HL = tval;
            break;
        case 0xEB: // XCHG instruction
            tval = REG_DE;
            REG_DE = REG_HL;
            REG_HL = tval;
            break;
        

        case 0xED: // LHLX instruction
            if (is8085())
            {
                REG_HL  = read8(REG_DE);
                REG_HL |= read8(REG_DE+1) << 8;
            }
            else
                opCALL(true);
            break;
        case 0xD9: // SHLX instruction (8085 only)
            if (is8085())
            {
                write8(REG_DE, REG_HL);
                write8(REG_DE+1, REG_HL >> 8);
            }
            else
                REG_PC = opPOP();
            break;

        // case 0x08: // DSUB instruction (8085 only)
        //     if (is8080())
        //         break;
        //     break;
       
        // case 0x10: // ASRH instruction (8085 only)
        //     if (is8080())
        //         break;
        //     afReg.ub.l = (afReg.ub.l & ~SR_CF) | (hlReg.ub.l & SR_CF);
        //     hlReg.uw = (hlReg.uw >> 1);
        //     break;
        
        // case 0x18: // RLDE instruction (8085 only)
        //     if (is8080())
        //         break;
        //     afReg.ub.l = (afReg.ub.l & ~(SR_CF | SR_VF)) | (deReg.ub.h >> 7);
        //     deReg.uw = (deReg.uw << 1) | (deReg.uw >> 15);
        //     if ((((deReg.uw >> 15) ^ afReg.ub.l) & SR_CF) != 0)
        //         afReg.ub.l |= SR_VF;
        //     break;

        // case 0x20: // RIM instruction (8085 only)
        //     if (is8080())
        //         break;
        //     break;

        // case 0x28: // LDEH nn instruction (8085 only)
        //     if (is8080())
        //         break;
        //     wzReg.uw = readArg8();
        //     deReg.uw = (hlReg.uw + wzReg.uw) & 0xFFFF;
        //     break;

        // case 0x30: // SIM instruction (8085 only)
        //     if (is8080())
        //         break;
        //     break;

        // case 0x38: // LDES nn instruction (8085 only)
        //     if (is8080())
        //         break;
        //     wzReg.uw = readArg8();
        //     deReg.uw = (spReg.uw + wzReg.uw) & 0xFFFF;
        //     break;

        // case 0xCB: // RST V instruction (8085 only)
        //     if (is8085())
        //     {
        //         if (afReg.ub.l & SR_VF)
        //             opRST(8);
        //     }
        //     else
        //         opJMP(true);
        //     break;

    }
}