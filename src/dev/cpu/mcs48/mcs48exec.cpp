// mcs48exec.cpp - MCS 48 Processor series package - Execute routines
//
// Author:  Tim Stark
// Date:    1/2/22

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "dev/cpu/mcs48/mcs48.h"

uint8_t mcs48_cpuDevice::read8i()
{
    uint16_t addr = pcReg;
    pcReg = ((pcReg + 1) & 0x7FF) | (pcReg & 0x800);
    return mapProgram.read8(addr);
}

uint8_t mcs48_cpuDevice::getP2Mask()
{
    uint8_t mask = 0xFF;
    if (cpuFlags & CPUF_UPI41)
    {
        if (flagsEnable)
            mask &= ~(P2_OBF|P2_NIBF);
        if (dmaEnable)
            mask &= ~(P2_DRQ|P2_NDACK);
    }
    return mask;
}

void mcs48_cpuDevice::expand(uint8_t port, opExpander op)
{
    write8port(2, p2Reg = (p2Reg & 0xF0) | (uint8_t(op) << 2) | (port & 0x03));

    // generate high-to-low transition on PROG line
    write8prog(0);

    if (op == eopRead)
    {
        // Read signals from P20-P23 lines
        // place P20-P23 in input mode
        write8port(2, p2Reg |= 0x0F);
        aReg = read8port(2) & 0x0F;
    }
    else
    {
        // Send signals to P20-P23 lines
        write8port(2, p2Reg = (p2Reg & 0xF0) | (aReg & 0x0F));
    }

    // generate low-to-high transition on PROG line
    write8prog(1);
}

void mcs48_cpuDevice::pushPCPSW()
{
    uint8_t sp = pswReg & PSW_SP;
    write8d(8 + sp*2, pcReg);
    write8d(9 + sp*2, ((pcReg >> 8) & 0x0F) | (pswReg & 0xF0));
    pswReg = (pswReg & 0xF0) | ((sp + 1) & PSW_SP);
}

void mcs48_cpuDevice::pullPCPSW()
{
    uint8_t sp = (pswReg - 1) & PSW_SP;
    pcReg =  read8d(8 + 2*sp) | (read8d(9 + 2*sp) << 8);
    pcReg &= irqInProgress ? 0x7FF : 0xFFF;
    pswReg = ((pcReg >> 8) & 0xF0) | sp;
}

void mcs48_cpuDevice::pullPC()
{
    uint8_t sp = (pswReg - 1) & PSW_SP;
    pcReg =  read8d(8 + 2*sp) | (read8d(9 + 2*sp) << 8);
    pcReg &= irqInProgress ? 0x7FF : 0xFFF;
    pswReg = (pswReg & 0xF0) | sp;
}

void mcs48_cpuDevice::exADD(uint8_t val)
{
    uint16_t temp = aReg + val;
    uint16_t temp4 = (aReg & 0x0F) | (val & 0x0F);

    // Update condition codes
    pswReg &= ~(PSW_C|PSW_A);
    pswReg |= (temp4 << 2) & PSW_A;
    pswReg |= (temp >> 1) & PSW_C;

    // Results
    aReg = temp;
}

void mcs48_cpuDevice::exADC(uint8_t val)
{
    uint8_t  carryin = (pswReg & PSW_C) >> 7;
    uint16_t temp = aReg + val + carryin;
    uint16_t temp4 = (aReg & 0x0F) | (val & 0x0F) + carryin;

    // Update condition codes
    pswReg &= ~(PSW_C|PSW_A);
    pswReg |= (temp4 << 2) & PSW_A;
    pswReg |= (temp >> 1) & PSW_C;

    // Results
    aReg = temp;
}

void mcs48_cpuDevice::exJUMP(uint16_t addr)
{
    pcReg = addr | (irqInProgress ? 0 : a11Reg);
}

void mcs48_cpuDevice::exJCC(bool flagRaised)
{
    uint16_t pch = pcReg & 0xF00;
    uint8_t offset = read8i();
    if (flagRaised)
        pcReg = pch | offset;
}

void mcs48_cpuDevice::exCALL(uint16_t addr)
{
    pushPCPSW();
    exJUMP(addr);
}

#define DO_OPC(name) void mcs48_cpuDevice::op##name()

DO_OPC(Illegal)
{
    eatCycles(1);
    fmt::printf("%s(MCS48): Illegal instruction (%02X) at PC %03X\n",
        getDeviceName(), read8p(fpcBase), fpcBase);
}

DO_OPC(ADD_A_R0)      { eatCycles(1); exADD(R0); }
DO_OPC(ADD_A_R1)      { eatCycles(1); exADD(R1); }
DO_OPC(ADD_A_R2)      { eatCycles(1); exADD(R2); }
DO_OPC(ADD_A_R3)      { eatCycles(1); exADD(R3); }
DO_OPC(ADD_A_R4)      { eatCycles(1); exADD(R4); }
DO_OPC(ADD_A_R5)      { eatCycles(1); exADD(R5); }
DO_OPC(ADD_A_R6)      { eatCycles(1); exADD(R6); }
DO_OPC(ADD_A_R7)      { eatCycles(1); exADD(R6); }
DO_OPC(ADD_A_XR0)     { eatCycles(1); exADD(read8d(R0)); }
DO_OPC(ADD_A_XR1)     { eatCycles(1); exADD(read8d(R1)); }
DO_OPC(ADD_A_N)       { eatCycles(2); exADD(read8i()); }

DO_OPC(ADC_A_R0)      { eatCycles(1); exADC(R0); }
DO_OPC(ADC_A_R1)      { eatCycles(1); exADC(R1); }
DO_OPC(ADC_A_R2)      { eatCycles(1); exADC(R2); }
DO_OPC(ADC_A_R3)      { eatCycles(1); exADC(R3); }
DO_OPC(ADC_A_R4)      { eatCycles(1); exADC(R4); }
DO_OPC(ADC_A_R5)      { eatCycles(1); exADC(R5); }
DO_OPC(ADC_A_R6)      { eatCycles(1); exADC(R6); }
DO_OPC(ADC_A_R7)      { eatCycles(1); exADC(R6); }
DO_OPC(ADC_A_XR0)     { eatCycles(1); exADC(read8d(R0)); }
DO_OPC(ADC_A_XR1)     { eatCycles(1); exADC(read8d(R1)); }
DO_OPC(ADC_A_N)       { eatCycles(2); exADC(read8i()); }

DO_OPC(ANL_A_R0)      { eatCycles(1); aReg &= R0; }
DO_OPC(ANL_A_R1)      { eatCycles(1); aReg &= R1; }
DO_OPC(ANL_A_R2)      { eatCycles(1); aReg &= R2; }
DO_OPC(ANL_A_R3)      { eatCycles(1); aReg &= R3; }
DO_OPC(ANL_A_R4)      { eatCycles(1); aReg &= R4; }
DO_OPC(ANL_A_R5)      { eatCycles(1); aReg &= R5; }
DO_OPC(ANL_A_R6)      { eatCycles(1); aReg &= R6; }
DO_OPC(ANL_A_R7)      { eatCycles(1); aReg &= R7; }
DO_OPC(ANL_A_XR0)     { eatCycles(1); aReg &= read8d(R0); }
DO_OPC(ANL_A_XR1)     { eatCycles(1); aReg &= read8d(R1); }
DO_OPC(ANL_A_N)       { eatCycles(2); aReg &= read8i(); }

DO_OPC(ANL_BUS_N)     { eatCycles(2); write8bus(read8bus() & read8i()); }
DO_OPC(ANL_P1_N)      { eatCycles(2); write8port(1, p1Reg &= read8i()); }
DO_OPC(ANL_P2_N)      { eatCycles(2); write8port(2, p2Reg &= read8i()); }
DO_OPC(ANLD_P4_A)     { eatCycles(2); expand(4, eopAND); }
DO_OPC(ANLD_P5_A)     { eatCycles(2); expand(5, eopAND); }
DO_OPC(ANLD_P6_A)     { eatCycles(2); expand(6, eopAND); }
DO_OPC(ANLD_P7_A)     { eatCycles(2); expand(7, eopAND); }

DO_OPC(CALL_0)        { eatCycles(2); exCALL(read8i() | 0x000); }
DO_OPC(CALL_1)        { eatCycles(2); exCALL(read8i() | 0x100); }
DO_OPC(CALL_2)        { eatCycles(2); exCALL(read8i() | 0x200); }
DO_OPC(CALL_3)        { eatCycles(2); exCALL(read8i() | 0x300); }
DO_OPC(CALL_4)        { eatCycles(2); exCALL(read8i() | 0x400); }
DO_OPC(CALL_5)        { eatCycles(2); exCALL(read8i() | 0x500); }
DO_OPC(CALL_6)        { eatCycles(2); exCALL(read8i() | 0x600); }
DO_OPC(CALL_7)        { eatCycles(2); exCALL(read8i() | 0x700); }

DO_OPC(CLR_A)         { eatCycles(1); aReg = 0; }
DO_OPC(CLR_C)         { eatCycles(1); pswReg &= ~PSW_C; }
DO_OPC(CLR_F0)        { eatCycles(1); pswReg &= ~PSW_F; }
DO_OPC(CLR_F1)        { eatCycles(1); f1Reg = false; }

DO_OPC(CPL_A)         { eatCycles(1); aReg ^= 0xFF; }
DO_OPC(CPL_C)         { eatCycles(1); pswReg ^= PSW_C; }
DO_OPC(CPL_F0)        { eatCycles(1); pswReg ^= PSW_F; }
DO_OPC(CPL_F1)        { eatCycles(1); f1Reg = !f1Reg; }

DO_OPC(DA_A)
{
    eatCycles(1);

    if ((aReg & 0x0F) > 0x09 || (pswReg & PSW_A))
    {
        if (aReg > 0xF9)
            pswReg |= PSW_C;
        aReg += 0x06;
    }
    
    if ((aReg & 0xF0) > 0x09 || (pswReg & PSW_C))
    {
        aReg += 0x60;
        pswReg |= PSW_C;
    }
}

DO_OPC(DEC_A)         { eatCycles(1); aReg--; }
DO_OPC(DEC_R0)        { eatCycles(1); R0--; }
DO_OPC(DEC_R1)        { eatCycles(1); R1--; }
DO_OPC(DEC_R2)        { eatCycles(1); R2--; }
DO_OPC(DEC_R3)        { eatCycles(1); R3--; }
DO_OPC(DEC_R4)        { eatCycles(1); R4--; }
DO_OPC(DEC_R5)        { eatCycles(1); R5--; }
DO_OPC(DEC_R6)        { eatCycles(1); R6--; }
DO_OPC(DEC_R7)        { eatCycles(1); R7--; }

DO_OPC(DIS_I)         { eatCycles(1); xirqEnable = false; }
DO_OPC(DIS_TCNTI)     { eatCycles(1); tirqEnable = false; timerOverflow = false; }

DO_OPC(DJNZ_R0)       { eatCycles(2); exJUMP(--R0 != 0); }
DO_OPC(DJNZ_R1)       { eatCycles(2); exJUMP(--R1 != 0); }
DO_OPC(DJNZ_R2)       { eatCycles(2); exJUMP(--R2 != 0); }
DO_OPC(DJNZ_R3)       { eatCycles(2); exJUMP(--R3 != 0); }
DO_OPC(DJNZ_R4)       { eatCycles(2); exJUMP(--R4 != 0); }
DO_OPC(DJNZ_R5)       { eatCycles(2); exJUMP(--R5 != 0); }
DO_OPC(DJNZ_R6)       { eatCycles(2); exJUMP(--R6 != 0); }
DO_OPC(DJNZ_R7)       { eatCycles(2); exJUMP(--R7 != 0); }

DO_OPC(EN_I)          { eatCycles(1); xirqEnable = true; }
DO_OPC(EN_TCNTI)      { eatCycles(1); tirqEnable = true; }
DO_OPC(EN_DMA)        { eatCycles(1); dmaEnable = true; write8port(2, p2Reg); }
DO_OPC(EN_FLAGS)      { eatCycles(1); flagsEnable = true; write8port(2, p2Reg); }
DO_OPC(ENT0_CLK)
{
    eatCycles(1);
    
}

DO_OPC(IN_A_P0)       { eatCycles(2); aReg = read8bus() & dbboReg; }
DO_OPC(IN_A_P1)       { eatCycles(2); aReg = read8port(1) & p1Reg; }
DO_OPC(IN_A_P2)       { eatCycles(2); aReg = read8port(2) & p2Reg; }
DO_OPC(INS_A_BUS)     { eatCycles(2); aReg = read8bus(); }
DO_OPC(IN_A_DBB)
{
    eatCycles(2);

    // Acknowledge the IBF IRQ and clear IBF bit in STS register
    // if (stsReg & STS_IBF) != 0);
        // IRQ callback
    stsReg &= ~STS_IBF;

    // If P2 flags are enabled, update P2 state
    if (flagsEnable && (p2Reg & P2_NIBF) == 0)
        write8port(2, p2Reg |= P2_NIBF);
    aReg = dbbiReg;
}

DO_OPC(INC_A)         { eatCycles(1); aReg++; }
DO_OPC(INC_R0)        { eatCycles(1); R0++; }
DO_OPC(INC_R1)        { eatCycles(1); R1++; }
DO_OPC(INC_R2)        { eatCycles(1); R2++; }
DO_OPC(INC_R3)        { eatCycles(1); R3++; }
DO_OPC(INC_R4)        { eatCycles(1); R4++; }
DO_OPC(INC_R5)        { eatCycles(1); R5++; }
DO_OPC(INC_R6)        { eatCycles(1); R6++; }
DO_OPC(INC_R7)        { eatCycles(1); R7++; }
DO_OPC(INC_XR0)       { eatCycles(1); write8d(R0, read8d(R0) + 1); }
DO_OPC(INC_XR1)       { eatCycles(1); write8d(R1, read8d(R1) + 1); }

DO_OPC(JB_0)          { eatCycles(2); exJCC((aReg & 0x01) != 0); }
DO_OPC(JB_1)          { eatCycles(2); exJCC((aReg & 0x02) != 0); }
DO_OPC(JB_2)          { eatCycles(2); exJCC((aReg & 0x04) != 0); }
DO_OPC(JB_3)          { eatCycles(2); exJCC((aReg & 0x08) != 0); }
DO_OPC(JB_4)          { eatCycles(2); exJCC((aReg & 0x10) != 0); }
DO_OPC(JB_5)          { eatCycles(2); exJCC((aReg & 0x20) != 0); }
DO_OPC(JB_6)          { eatCycles(2); exJCC((aReg & 0x40) != 0); }
DO_OPC(JB_7)          { eatCycles(2); exJCC((aReg & 0x80) != 0); }
DO_OPC(JC)            { eatCycles(2); exJCC((pswReg & PSW_C) != 0); }
DO_OPC(JF0)           { eatCycles(2); exJCC((pswReg & PSW_F) != 0); }
DO_OPC(JF1)           { eatCycles(2); exJCC(f1Reg); }
DO_OPC(JNC)           { eatCycles(2); exJCC((pswReg & PSW_C) == 0); }
DO_OPC(JNI)           { eatCycles(2); irqPolled = (irqState == 0); exJCC(irqState != 0); }
DO_OPC(JNIBF)         { eatCycles(2); irqPolled = (stsReg & STS_IBF) != 0; exJCC((stsReg & STS_IBF) == 0); }
DO_OPC(JNT_0)         { eatCycles(2); exJCC(read8test(0) == 0); }
DO_OPC(JNT_1)         { eatCycles(2); exJCC(read8test(1) == 0); }
DO_OPC(JNZ)           { eatCycles(2); exJCC(aReg != 0); }
DO_OPC(JOBF)          { eatCycles(2); exJCC((stsReg & STS_OBF) != 0); }
DO_OPC(JTF)           { eatCycles(2); exJCC(timerFlag); timerFlag = false; }
DO_OPC(JT_0)          { eatCycles(2); exJCC(read8test(0) != 0); }
DO_OPC(JT_1)          { eatCycles(2); exJCC(read8test(1) != 0); }
DO_OPC(JZ)            { eatCycles(2); exJCC(aReg == 0); }

DO_OPC(JMP_0)         { eatCycles(2); exJUMP(read8i() | 0x000); }
DO_OPC(JMP_1)         { eatCycles(2); exJUMP(read8i() | 0x100); }
DO_OPC(JMP_2)         { eatCycles(2); exJUMP(read8i() | 0x200); }
DO_OPC(JMP_3)         { eatCycles(2); exJUMP(read8i() | 0x300); }
DO_OPC(JMP_4)         { eatCycles(2); exJUMP(read8i() | 0x400); }
DO_OPC(JMP_5)         { eatCycles(2); exJUMP(read8i() | 0x500); }
DO_OPC(JMP_6)         { eatCycles(2); exJUMP(read8i() | 0x600); }
DO_OPC(JMP_7)         { eatCycles(2); exJUMP(read8i() | 0x700); }
DO_OPC(JMPP_XA)       { eatCycles(2); pcReg &= 0xF00; pcReg |= read8p(pcReg | aReg); }

DO_OPC(MOV_A_N)       { eatCycles(2); aReg = read8i(); }
DO_OPC(MOV_A_PSW)     { eatCycles(1); aReg = pswReg | 0x08; }
DO_OPC(MOV_A_R0)      { eatCycles(1); aReg = R0; }
DO_OPC(MOV_A_R1)      { eatCycles(1); aReg = R1; }
DO_OPC(MOV_A_R2)      { eatCycles(1); aReg = R2; }
DO_OPC(MOV_A_R3)      { eatCycles(1); aReg = R3; }
DO_OPC(MOV_A_R4)      { eatCycles(1); aReg = R4; }
DO_OPC(MOV_A_R5)      { eatCycles(1); aReg = R5; }
DO_OPC(MOV_A_R6)      { eatCycles(1); aReg = R6; }
DO_OPC(MOV_A_R7)      { eatCycles(1); aReg = R7; }
DO_OPC(MOV_A_XR0)     { eatCycles(1); aReg = read8d(R0); }
DO_OPC(MOV_A_XR1)     { eatCycles(1); aReg = read8d(R1); }
DO_OPC(MOV_A_T)       { eatCycles(1); aReg = timerReg; }

DO_OPC(MOV_PSW_A)     { eatCycles(1); pswReg = aReg & ~0x08; updateRegisters(); }
DO_OPC(MOV_STS_A)     { eatCycles(1); stsReg = (stsReg & 0x0F) | (aReg & 0xF0); }
DO_OPC(MOV_R0_A)      { eatCycles(1); R0 = aReg; }
DO_OPC(MOV_R1_A)      { eatCycles(1); R1 = aReg; }
DO_OPC(MOV_R2_A)      { eatCycles(1); R2 = aReg; }
DO_OPC(MOV_R3_A)      { eatCycles(1); R3 = aReg; }
DO_OPC(MOV_R4_A)      { eatCycles(1); R4 = aReg; }
DO_OPC(MOV_R5_A)      { eatCycles(1); R5 = aReg; }
DO_OPC(MOV_R6_A)      { eatCycles(1); R6 = aReg; }
DO_OPC(MOV_R7_A)      { eatCycles(1); R7 = aReg; }
DO_OPC(MOV_R0_N)      { eatCycles(2); R0 = read8i(); }
DO_OPC(MOV_R1_N)      { eatCycles(2); R1 = read8i(); }
DO_OPC(MOV_R2_N)      { eatCycles(2); R2 = read8i(); }
DO_OPC(MOV_R3_N)      { eatCycles(2); R3 = read8i(); }
DO_OPC(MOV_R4_N)      { eatCycles(2); R4 = read8i(); }
DO_OPC(MOV_R5_N)      { eatCycles(2); R5 = read8i(); }
DO_OPC(MOV_R6_N)      { eatCycles(2); R6 = read8i(); }
DO_OPC(MOV_R7_N)      { eatCycles(2); R7 = read8i(); }
DO_OPC(MOV_T_A)       { eatCycles(1); timerReg = aReg; }
DO_OPC(MOV_XR0_A)     { eatCycles(1); write8d(R0, aReg); }
DO_OPC(MOV_XR1_A)     { eatCycles(1); write8d(R1, aReg); }
DO_OPC(MOV_XR0_N)     { eatCycles(2); write8d(R0, read8i()); }
DO_OPC(MOV_XR1_N)     { eatCycles(2); write8d(R1, read8i()); }

DO_OPC(MOVD_A_P4)     { eatCycles(2); expand(4, eopRead); }
DO_OPC(MOVD_A_P5)     { eatCycles(2); expand(5, eopRead); }
DO_OPC(MOVD_A_P6)     { eatCycles(2); expand(6, eopRead); }
DO_OPC(MOVD_A_P7)     { eatCycles(2); expand(7, eopRead); }
DO_OPC(MOVD_P4_A)     { eatCycles(2); expand(4, eopWrite); }
DO_OPC(MOVD_P5_A)     { eatCycles(2); expand(5, eopWrite); }
DO_OPC(MOVD_P6_A)     { eatCycles(2); expand(6, eopWrite); }
DO_OPC(MOVD_P7_A)     { eatCycles(2); expand(7, eopWrite); }

DO_OPC(MOVP_A_XA)     { eatCycles(2); aReg = read8p((pcReg & 0xF00) | aReg); }
DO_OPC(MOVP3_A_XA)    { eatCycles(2); aReg = read8p(0x300 | aReg); }

DO_OPC(MOVX_A_XR0)    { eatCycles(2); aReg = read8io(R0); }
DO_OPC(MOVX_A_XR1)    { eatCycles(2); aReg = read8io(R1); }
DO_OPC(MOVX_XR0_A)    { eatCycles(2); write8io(R0, aReg); }
DO_OPC(MOVX_XR1_A)    { eatCycles(2); write8io(R1, aReg); }

DO_OPC(NOP)           { eatCycles(1); }

DO_OPC(ORL_A_R0)      { eatCycles(1); aReg |= R0; }
DO_OPC(ORL_A_R1)      { eatCycles(1); aReg |= R1; }
DO_OPC(ORL_A_R2)      { eatCycles(1); aReg |= R2; }
DO_OPC(ORL_A_R3)      { eatCycles(1); aReg |= R3; }
DO_OPC(ORL_A_R4)      { eatCycles(1); aReg |= R4; }
DO_OPC(ORL_A_R5)      { eatCycles(1); aReg |= R5; }
DO_OPC(ORL_A_R6)      { eatCycles(1); aReg |= R6; }
DO_OPC(ORL_A_R7)      { eatCycles(1); aReg |= R7; }
DO_OPC(ORL_A_XR0)     { eatCycles(1); aReg |= read8d(R0); }
DO_OPC(ORL_A_XR1)     { eatCycles(1); aReg |= read8d(R1); }
DO_OPC(ORL_A_N)       { eatCycles(2); aReg |= read8i(); }

DO_OPC(ORL_BUS_N)     { eatCycles(2); write8bus(read8bus() | read8i()); }
DO_OPC(ORL_P1_N)      { eatCycles(2); write8port(1, p1Reg |= read8i()); }
DO_OPC(ORL_P2_N)      { eatCycles(2); write8port(2, p2Reg |= read8i() & getP2Mask()); }
DO_OPC(ORLD_P4_A)     { eatCycles(2); expand(4, eopOR); }
DO_OPC(ORLD_P5_A)     { eatCycles(2); expand(5, eopOR); }
DO_OPC(ORLD_P6_A)     { eatCycles(2); expand(6, eopOR); }
DO_OPC(ORLD_P7_A)     { eatCycles(2); expand(7, eopOR); }

DO_OPC(OUTL_BUS_A)    { eatCycles(2); write8bus(aReg); }
DO_OPC(OUTL_P0_A)     { eatCycles(2); write8bus(dbboReg = aReg); }
DO_OPC(OUTL_P1_A)     { eatCycles(2); write8port(1, p1Reg = aReg); }
DO_OPC(OUTL_P2_A)     { eatCycles(2); uint8_t mask = getP2Mask(); write8port(2, p2Reg = (p2Reg & ~mask) | (aReg & mask)); }
DO_OPC(OUT_DBB_A)
{
    eatCycles(2);

    // load DBBO and update OBF bit in STS register
    dbboReg = aReg;
    stsReg |= STS_OBF;

    // If P2 flags are enabled, update P2 state
    if (flagsEnable && (p2Reg & P2_OBF) == 0)
        write8port(2, p2Reg |= P2_OBF);
}

DO_OPC(RET)           { eatCycles(1); pullPC(); }
DO_OPC(RETR)          { eatCycles(2); irqInProgress = false; pullPCPSW(); }

DO_OPC(RL_A)          { eatCycles(1); aReg = (aReg << 1) | (aReg >> 7); }
DO_OPC(RLC_A)         { eatCycles(1); uint8_t carry = aReg & PSW_C; 
                        aReg = (aReg << 1) | (aReg >> 7);
                        pswReg = (pswReg & ~PSW_C) | carry; }

DO_OPC(RR_A)          { eatCycles(1); aReg = (aReg >> 1) | (aReg << 7); }
DO_OPC(RRC_A)         { eatCycles(1); uint8_t carry = (aReg << 7) & PSW_C;
                        aReg = (aReg >> 1) | (aReg << 7); 
                        pswReg = (pswReg & ~PSW_C) | carry; }

DO_OPC(SEL_MB0)       { eatCycles(1); a11Reg = 0x000; }
DO_OPC(SEL_MB1)       { eatCycles(1); a11Reg = 0x800; }
DO_OPC(SEL_RB0)       { eatCycles(1); pswReg &= ~PSW_B; updateRegisters(); }
DO_OPC(SEL_RB1)       { eatCycles(1); pswReg |= PSW_B; updateRegisters(); }

DO_OPC(STOP_TCNT)     { eatCycles(1);  }
DO_OPC(STOP_T)        { eatCycles(1);  }
DO_OPC(STRT_CNT)
{
    eatCycles(1);
}

DO_OPC(SWAP_A)        { eatCycles(1); aReg = (aReg << 4) | (aReg >> 4); }

DO_OPC(XCH_A_R0)      { eatCycles(1); uint8_t tmp = aReg; aReg = R0; R0 = tmp; }
DO_OPC(XCH_A_R1)      { eatCycles(1); uint8_t tmp = aReg; aReg = R1; R1 = tmp; }
DO_OPC(XCH_A_R2)      { eatCycles(1); uint8_t tmp = aReg; aReg = R2; R2 = tmp; }
DO_OPC(XCH_A_R3)      { eatCycles(1); uint8_t tmp = aReg; aReg = R3; R3 = tmp; }
DO_OPC(XCH_A_R4)      { eatCycles(1); uint8_t tmp = aReg; aReg = R4; R4 = tmp; }
DO_OPC(XCH_A_R5)      { eatCycles(1); uint8_t tmp = aReg; aReg = R5; R5 = tmp; }
DO_OPC(XCH_A_R6)      { eatCycles(1); uint8_t tmp = aReg; aReg = R6; R6 = tmp; }
DO_OPC(XCH_A_R7)      { eatCycles(1); uint8_t tmp = aReg; aReg = R7; R7 = tmp; }
DO_OPC(XCH_A_XR0)     { eatCycles(1); uint8_t tmp = aReg; aReg = read8d(R0); write8d(R0, tmp); }
DO_OPC(XCH_A_XR1)     { eatCycles(1); uint8_t tmp = aReg; aReg = read8d(R1); write8d(R1, tmp); }

DO_OPC(XCHD_A_XR0)    { eatCycles(1); uint8_t tmp = read8d(R0);
                        write8d(R0, (tmp & 0xF0) | (aReg & 0x0F));
                        aReg = (aReg & 0xF0) | (tmp & 0x0F); }

DO_OPC(XCHD_A_XR1)    { eatCycles(1); uint8_t tmp = read8d(R1);
                        write8d(R1, (tmp & 0xF0) | (aReg & 0x0F));
                        aReg = (aReg & 0xF0) | (tmp & 0x0F); }

DO_OPC(XRL_A_R0)      { eatCycles(1); aReg ^= R0; }
DO_OPC(XRL_A_R1)      { eatCycles(1); aReg ^= R1; }
DO_OPC(XRL_A_R2)      { eatCycles(1); aReg ^= R2; }
DO_OPC(XRL_A_R3)      { eatCycles(1); aReg ^= R3; }
DO_OPC(XRL_A_R4)      { eatCycles(1); aReg ^= R4; }
DO_OPC(XRL_A_R5)      { eatCycles(1); aReg ^= R5; }
DO_OPC(XRL_A_R6)      { eatCycles(1); aReg ^= R6; }
DO_OPC(XRL_A_R7)      { eatCycles(1); aReg ^= R7; }
DO_OPC(XRL_A_XR0)     { eatCycles(1); aReg ^= read8d(R0); }
DO_OPC(XRL_A_XR1)     { eatCycles(1); aReg ^= read8d(R1); }
DO_OPC(XRL_A_N)       { eatCycles(2); aReg ^= read8i(); }
