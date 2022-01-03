// mcs48.cpp - MCS 48 Processor series package
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

#define DO_OPC(name) void mcs48_cpuDevice::name()

DO_OPC(illegal)
{
    eatCycles(1);
    fmt::printf("%s(MCS48): Illegal instruction (%02X) at PC %03X\n",
        getDeviceName(), read8p(fpcBase), fpcBase);
}

DO_OPC(opADD_A_R0)      { eatCycles(1); exADD(R0); }
DO_OPC(opADD_A_R1)      { eatCycles(1); exADD(R1); }
DO_OPC(opADD_A_R2)      { eatCycles(1); exADD(R2); }
DO_OPC(opADD_A_R3)      { eatCycles(1); exADD(R3); }
DO_OPC(opADD_A_R4)      { eatCycles(1); exADD(R4); }
DO_OPC(opADD_A_R5)      { eatCycles(1); exADD(R5); }
DO_OPC(opADD_A_R6)      { eatCycles(1); exADD(R6); }
DO_OPC(opADD_A_R7)      { eatCycles(1); exADD(R6); }
DO_OPC(opADD_A_XR0)     { eatCycles(1); exADD(read8d(R0)); }
DO_OPC(opADD_A_XR1)     { eatCycles(1); exADD(read8d(R1)); }
DO_OPC(opADD_A_N)       { eatCycles(2); exADD(read8i()); }

DO_OPC(opADC_A_R0)      { eatCycles(1); exADC(R0); }
DO_OPC(opADC_A_R1)      { eatCycles(1); exADC(R1); }
DO_OPC(opADC_A_R2)      { eatCycles(1); exADC(R2); }
DO_OPC(opADC_A_R3)      { eatCycles(1); exADC(R3); }
DO_OPC(opADC_A_R4)      { eatCycles(1); exADC(R4); }
DO_OPC(opADC_A_R5)      { eatCycles(1); exADC(R5); }
DO_OPC(opADC_A_R6)      { eatCycles(1); exADC(R6); }
DO_OPC(opADC_A_R7)      { eatCycles(1); exADC(R6); }
DO_OPC(opADC_A_XR0)     { eatCycles(1); exADC(read8d(R0)); }
DO_OPC(opADC_A_XR1)     { eatCycles(1); exADC(read8d(R1)); }
DO_OPC(opADC_A_N)       { eatCycles(2); exADC(read8i()); }

DO_OPC(opANL_A_R0)      { eatCycles(1); aReg &= R0; }
DO_OPC(opANL_A_R1)      { eatCycles(1); aReg &= R1; }
DO_OPC(opANL_A_R2)      { eatCycles(1); aReg &= R2; }
DO_OPC(opANL_A_R3)      { eatCycles(1); aReg &= R3; }
DO_OPC(opANL_A_R4)      { eatCycles(1); aReg &= R4; }
DO_OPC(opANL_A_R5)      { eatCycles(1); aReg &= R5; }
DO_OPC(opANL_A_R6)      { eatCycles(1); aReg &= R6; }
DO_OPC(opANL_A_R7)      { eatCycles(1); aReg &= R7; }
DO_OPC(opANL_A_XR0)     { eatCycles(1); aReg &= read8d(R0); }
DO_OPC(opANL_A_XR1)     { eatCycles(1); aReg &= read8d(R1); }
DO_OPC(opANL_A_N)       { eatCycles(2); aReg &= read8i(); }

DO_OPC(opANL_BUS_N)     { eatCycles(2); write8bus(read8bus() & read8i()); }
DO_OPC(opANL_P1_N)      { eatCycles(2); write8port(1, p1Reg &= read8i()); }
DO_OPC(opANL_P2_N)      { eatCycles(2); write8port(2, p2Reg &= read8i()); }
DO_OPC(opANLD_P4_A)     { eatCycles(2); expand(4, eopAND); }
DO_OPC(opANLD_P5_A)     { eatCycles(2); expand(5, eopAND); }
DO_OPC(opANLD_P6_A)     { eatCycles(2); expand(6, eopAND); }
DO_OPC(opANLD_P7_A)     { eatCycles(2); expand(7, eopAND); }

DO_OPC(opCALL_0)        { eatCycles(2); exCALL(read8i() | 0x000); }
DO_OPC(opCALL_1)        { eatCycles(2); exCALL(read8i() | 0x100); }
DO_OPC(opCALL_2)        { eatCycles(2); exCALL(read8i() | 0x200); }
DO_OPC(opCALL_3)        { eatCycles(2); exCALL(read8i() | 0x300); }
DO_OPC(opCALL_4)        { eatCycles(2); exCALL(read8i() | 0x400); }
DO_OPC(opCALL_5)        { eatCycles(2); exCALL(read8i() | 0x500); }
DO_OPC(opCALL_6)        { eatCycles(2); exCALL(read8i() | 0x600); }
DO_OPC(opCALL_7)        { eatCycles(2); exCALL(read8i() | 0x700); }

DO_OPC(opCLR_A)         { eatCycles(1); aReg = 0; }
DO_OPC(opCLR_C)         { eatCycles(1); pswReg &= ~PSW_C; }
DO_OPC(opCLR_F0)        { eatCycles(1); pswReg &= ~PSW_F; }
DO_OPC(opCLR_F1)        { eatCycles(1); f1Reg = false; }

DO_OPC(opCPL_A)         { eatCycles(1); aReg ^= 0xFF; }
DO_OPC(opCPL_C)         { eatCycles(1); pswReg ^= PSW_C; }
DO_OPC(opCPL_F0)        { eatCycles(1); pswReg ^= PSW_F; }
DO_OPC(opCPL_F1)        { eatCycles(1); f1Reg = !f1Reg; }

DO_OPC(opDA_A)
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

DO_OPC(opDEC_A)         { eatCycles(1); aReg--; }
DO_OPC(opDEC_R0)        { eatCycles(1); R0--; }
DO_OPC(opDEC_R1)        { eatCycles(1); R1--; }
DO_OPC(opDEC_R2)        { eatCycles(1); R2--; }
DO_OPC(opDEC_R3)        { eatCycles(1); R3--; }
DO_OPC(opDEC_R4)        { eatCycles(1); R4--; }
DO_OPC(opDEC_R5)        { eatCycles(1); R5--; }
DO_OPC(opDEC_R6)        { eatCycles(1); R6--; }
DO_OPC(opDEC_R7)        { eatCycles(1); R7--; }

DO_OPC(opDIS_I)         { eatCycles(1); xirqEnable = false; }
DO_OPC(opDIS_TCNTI)     { eatCycles(1); tirqEnable = false; timerOverflow = false; }

DO_OPC(opDJNZ_R0)       { eatCycles(2); exJUMP(--R0 != 0); }
DO_OPC(opDJNZ_R1)       { eatCycles(2); exJUMP(--R1 != 0); }
DO_OPC(opDJNZ_R2)       { eatCycles(2); exJUMP(--R2 != 0); }
DO_OPC(opDJNZ_R3)       { eatCycles(2); exJUMP(--R3 != 0); }
DO_OPC(opDJNZ_R4)       { eatCycles(2); exJUMP(--R4 != 0); }
DO_OPC(opDJNZ_R5)       { eatCycles(2); exJUMP(--R5 != 0); }
DO_OPC(opDJNZ_R6)       { eatCycles(2); exJUMP(--R6 != 0); }
DO_OPC(opDJNZ_R7)       { eatCycles(2); exJUMP(--R7 != 0); }

DO_OPC(opEN_I)          { eatCycles(1); xirqEnable = true; }
DO_OPC(opEN_TCNTI)      { eatCycles(1); tirqEnable = true; }
DO_OPC(opEN_DMA)        { eatCycles(1); dmaEnable = true; write8port(2, p2Reg); }
DO_OPC(opEN_FLAGS)      { eatCycles(1); flagsEnable = true; write8port(2, p2Reg); }
DO_OPC(opENT0_CLK)
{
    eatCycles(1);
    
}

DO_OPC(opIN_A_P0)       { eatCycles(2); aReg = read8bus() & dbboReg; }
DO_OPC(opIN_A_P1)       { eatCycles(2); aReg = read8port(1) & p1Reg; }
DO_OPC(opIN_A_P2)       { eatCycles(2); aReg = read8port(2) & p2Reg; }
DO_OPC(opINS_A_BUS)     { eatCycles(2); aReg = read8bus(); }
DO_OPC(opIN_A_DBB)
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

DO_OPC(opINC_A)         { eatCycles(1); aReg++; }
DO_OPC(opINC_R0)        { eatCycles(1); R0++; }
DO_OPC(opINC_R1)        { eatCycles(1); R1++; }
DO_OPC(opINC_R2)        { eatCycles(1); R2++; }
DO_OPC(opINC_R3)        { eatCycles(1); R3++; }
DO_OPC(opINC_R4)        { eatCycles(1); R4++; }
DO_OPC(opINC_R5)        { eatCycles(1); R5++; }
DO_OPC(opINC_R6)        { eatCycles(1); R6++; }
DO_OPC(opINC_R7)        { eatCycles(1); R7++; }
DO_OPC(opINC_XR0)       { eatCycles(1); write8d(R0, read8d(R0) + 1); }
DO_OPC(opINC_XR1)       { eatCycles(1); write8d(R1, read8d(R1) + 1); }

DO_OPC(opJB_0)          { eatCycles(2); exJCC((aReg & 0x01) != 0); }
DO_OPC(opJB_1)          { eatCycles(2); exJCC((aReg & 0x02) != 0); }
DO_OPC(opJB_2)          { eatCycles(2); exJCC((aReg & 0x04) != 0); }
DO_OPC(opJB_3)          { eatCycles(2); exJCC((aReg & 0x08) != 0); }
DO_OPC(opJB_4)          { eatCycles(2); exJCC((aReg & 0x10) != 0); }
DO_OPC(opJB_5)          { eatCycles(2); exJCC((aReg & 0x20) != 0); }
DO_OPC(opJB_6)          { eatCycles(2); exJCC((aReg & 0x40) != 0); }
DO_OPC(opJB_7)          { eatCycles(2); exJCC((aReg & 0x80) != 0); }
DO_OPC(opJC)            { eatCycles(2); exJCC((pswReg & PSW_C) != 0); }
DO_OPC(opJF0)           { eatCycles(2); exJCC((pswReg & PSW_F) != 0); }
DO_OPC(opJF1)           { eatCycles(2); exJCC(f1Reg); }
DO_OPC(opJNC)           { eatCycles(2); exJCC((pswReg & PSW_C) == 0); }
DO_OPC(opJNI)           { eatCycles(2); irqPolled = (irqState == 0); exJCC(irqState != 0); }
DO_OPC(opJNIBF)         { eatCycles(2); irqPolled = (stsReg & STS_IBF) != 0; exJCC((stsReg & STS_IBF) == 0); }
DO_OPC(opJNT_0)         { eatCycles(2); exJCC(read8test(0) == 0); }
DO_OPC(opJNT_1)         { eatCycles(2); exJCC(read8test(1) == 0); }
DO_OPC(opJNZ)           { eatCycles(2); exJCC(aReg != 0); }
DO_OPC(opJOBF)          { eatCycles(2); exJCC((stsReg & STS_OBF) != 0); }
DO_OPC(opJTF)           { eatCycles(2); exJCC(timerFlag); timerFlag = false; }
DO_OPC(opJT_0)          { eatCycles(2); exJCC(read8test(0) != 0); }
DO_OPC(opJT_1)          { eatCycles(2); exJCC(read8test(1) != 0); }
DO_OPC(opJZ)            { eatCycles(2); exJCC(aReg == 0); }

DO_OPC(opJMP_0)         { eatCycles(2); exJUMP(read8i() | 0x000); }
DO_OPC(opJMP_1)         { eatCycles(2); exJUMP(read8i() | 0x100); }
DO_OPC(opJMP_2)         { eatCycles(2); exJUMP(read8i() | 0x200); }
DO_OPC(opJMP_3)         { eatCycles(2); exJUMP(read8i() | 0x300); }
DO_OPC(opJMP_4)         { eatCycles(2); exJUMP(read8i() | 0x400); }
DO_OPC(opJMP_5)         { eatCycles(2); exJUMP(read8i() | 0x500); }
DO_OPC(opJMP_6)         { eatCycles(2); exJUMP(read8i() | 0x600); }
DO_OPC(opJMP_7)         { eatCycles(2); exJUMP(read8i() | 0x700); }
DO_OPC(opJMPP_XA)       { eatCycles(2); pcReg &= 0xF00; pcReg |= read8p(pcReg | aReg); }

DO_OPC(opMOV_A_N)       { eatCycles(2); aReg = read8i(); }
DO_OPC(opMOV_A_PSW)     { eatCycles(1); aReg = pswReg | 0x08; }
DO_OPC(opMOV_A_R0)      { eatCycles(1); aReg = R0; }
DO_OPC(opMOV_A_R1)      { eatCycles(1); aReg = R1; }
DO_OPC(opMOV_A_R2)      { eatCycles(1); aReg = R2; }
DO_OPC(opMOV_A_R3)      { eatCycles(1); aReg = R3; }
DO_OPC(opMOV_A_R4)      { eatCycles(1); aReg = R4; }
DO_OPC(opMOV_A_R5)      { eatCycles(1); aReg = R5; }
DO_OPC(opMOV_A_R6)      { eatCycles(1); aReg = R6; }
DO_OPC(opMOV_A_R7)      { eatCycles(1); aReg = R7; }
DO_OPC(opMOV_A_XR0)     { eatCycles(1); aReg = read8d(R0); }
DO_OPC(opMOV_A_XR1)     { eatCycles(1); aReg = read8d(R1); }
DO_OPC(opMOV_A_T)       { eatCycles(1); aReg = timerReg; }

DO_OPC(opMOV_PSW_A)     { eatCycles(1); pswReg = aReg & ~0x08; updateRegisters(); }
DO_OPC(opMOV_STS_A)     { eatCycles(1); stsReg = (stsReg & 0x0F) | (aReg & 0xF0); }
DO_OPC(opMOV_R0_A)      { eatCycles(1); R0 = aReg; }
DO_OPC(opMOV_R1_A)      { eatCycles(1); R1 = aReg; }
DO_OPC(opMOV_R2_A)      { eatCycles(1); R2 = aReg; }
DO_OPC(opMOV_R3_A)      { eatCycles(1); R3 = aReg; }
DO_OPC(opMOV_R4_A)      { eatCycles(1); R4 = aReg; }
DO_OPC(opMOV_R5_A)      { eatCycles(1); R5 = aReg; }
DO_OPC(opMOV_R6_A)      { eatCycles(1); R6 = aReg; }
DO_OPC(opMOV_R7_A)      { eatCycles(1); R7 = aReg; }
DO_OPC(opMOV_R0_N)      { eatCycles(2); R0 = read8i(); }
DO_OPC(opMOV_R1_N)      { eatCycles(2); R1 = read8i(); }
DO_OPC(opMOV_R2_N)      { eatCycles(2); R2 = read8i(); }
DO_OPC(opMOV_R3_N)      { eatCycles(2); R3 = read8i(); }
DO_OPC(opMOV_R4_N)      { eatCycles(2); R4 = read8i(); }
DO_OPC(opMOV_R5_N)      { eatCycles(2); R5 = read8i(); }
DO_OPC(opMOV_R6_N)      { eatCycles(2); R6 = read8i(); }
DO_OPC(opMOV_R7_N)      { eatCycles(2); R7 = read8i(); }
DO_OPC(opMOV_T_A)       { eatCycles(1); timerReg = aReg; }
DO_OPC(opMOV_XR0_A)     { eatCycles(1); write8d(R0, aReg); }
DO_OPC(opMOV_XR1_A)     { eatCycles(1); write8d(R1, aReg); }
DO_OPC(opMOV_XR0_N)     { eatCycles(2); write8d(R0, read8i()); }
DO_OPC(opMOV_XR1_N)     { eatCycles(2); write8d(R1, read8i()); }

DO_OPC(opMOVD_A_P4)     { eatCycles(2); expand(4, eopRead); }
DO_OPC(opMOVD_A_P5)     { eatCycles(2); expand(5, eopRead); }
DO_OPC(opMOVD_A_P6)     { eatCycles(2); expand(6, eopRead); }
DO_OPC(opMOVD_A_P7)     { eatCycles(2); expand(7, eopRead); }
DO_OPC(opMOVD_P4_A)     { eatCycles(2); expand(4, eopWrite); }
DO_OPC(opMOVD_P5_A)     { eatCycles(2); expand(5, eopWrite); }
DO_OPC(opMOVD_P6_A)     { eatCycles(2); expand(6, eopWrite); }
DO_OPC(opMOVD_P7_A)     { eatCycles(2); expand(7, eopWrite); }

DO_OPC(opMOVP_A_XA)     { eatCycles(2); aReg = read8p((pcReg & 0xF00) | aReg); }
DO_OPC(opMOVP3_A_XA)    { eatCycles(2); aReg = read8p(0x300 | aReg); }

DO_OPC(opMOVX_A_XR0)    { eatCycles(2); aReg = read8io(R0); }
DO_OPC(opMOVX_A_XR1)    { eatCycles(2); aReg = read8io(R1); }
DO_OPC(opMOVX_XR0_A)    { eatCycles(2); write8io(R0, aReg); }
DO_OPC(opMOVX_XR1_A)    { eatCycles(2); write8io(R1, aReg); }

DO_OPC(opNOP)           { eatCycles(1); }

DO_OPC(opORL_A_R0)      { eatCycles(1); aReg |= R0; }
DO_OPC(opORL_A_R1)      { eatCycles(1); aReg |= R1; }
DO_OPC(opORL_A_R2)      { eatCycles(1); aReg |= R2; }
DO_OPC(opORL_A_R3)      { eatCycles(1); aReg |= R3; }
DO_OPC(opORL_A_R4)      { eatCycles(1); aReg |= R4; }
DO_OPC(opORL_A_R5)      { eatCycles(1); aReg |= R5; }
DO_OPC(opORL_A_R6)      { eatCycles(1); aReg |= R6; }
DO_OPC(opORL_A_R7)      { eatCycles(1); aReg |= R7; }
DO_OPC(opORL_A_XR0)     { eatCycles(1); aReg |= read8d(R0); }
DO_OPC(opORL_A_XR1)     { eatCycles(1); aReg |= read8d(R1); }
DO_OPC(opORL_A_N)       { eatCycles(2); aReg |= read8i(); }

DO_OPC(opORL_BUS_N)     { eatCycles(2); write8bus(read8bus() | read8i()); }
DO_OPC(opORL_P1_N)      { eatCycles(2); write8port(1, p1Reg |= read8i()); }
DO_OPC(opORL_P2_N)      { eatCycles(2); write8port(2, p2Reg |= read8i() & getP2Mask()); }
DO_OPC(opORLD_P4_A)     { eatCycles(2); expand(4, eopOR); }
DO_OPC(opORLD_P5_A)     { eatCycles(2); expand(5, eopOR); }
DO_OPC(opORLD_P6_A)     { eatCycles(2); expand(6, eopOR); }
DO_OPC(opORLD_P7_A)     { eatCycles(2); expand(7, eopOR); }

DO_OPC(opOUTL_BUS_A)    { eatCycles(2); write8bus(aReg); }
DO_OPC(opOUTL_P0_A)     { eatCycles(2); write8bus(dbboReg = aReg); }
DO_OPC(opOUTL_P1_A)     { eatCycles(2); write8port(1, p1Reg = aReg); }
DO_OPC(opOUTL_P2_A)     { eatCycles(2); uint8_t mask = getP2Mask(); write8port(2, p2Reg = (p2Reg & ~mask) | (aReg & mask)); }
DO_OPC(opOUT_DBB_A)
{
    eatCycles(2);

    // load DBBO and update OBF bit in STS register
    dbboReg = aReg;
    stsReg |= STS_OBF;

    // If P2 flags are enabled, update P2 state
    if (flagsEnable && (p2Reg & P2_OBF) == 0)
        write8port(2, p2Reg |= P2_OBF);
}

DO_OPC(opRET)            { eatCycles(1); pullPC(); }
DO_OPC(opRETR)           { eatCycles(2); irqInProgress = false; pullPCPSW(); }

DO_OPC(opRL_A)           { eatCycles(1); aReg = (aReg << 1) | (aReg >> 7); }
DO_OPC(opRLC_A)          { eatCycles(1); uint8_t carry = aReg & PSW_C; 
                           aReg = (aReg << 1) | (aReg >> 7);
                           pswReg = (pswReg & ~PSW_C) | carry; }

DO_OPC(opRR_A)           { eatCycles(1); aReg = (aReg >> 1) | (aReg << 7); }
DO_OPC(opRRC_A)          { eatCycles(1); uint8_t carry = (aReg << 7) & PSW_C;
                           aReg = (aReg >> 1) | (aReg << 7); 
                           pswReg = (pswReg & ~PSW_C) | carry; }

DO_OPC(opSEL_MB0)        { eatCycles(1); a11Reg = 0x000; }
DO_OPC(opSEL_MB1)        { eatCycles(1); a11Reg = 0x800; }
DO_OPC(opSEL_RB0)        { eatCycles(1); pswReg &= ~PSW_B; updateRegisters(); }
DO_OPC(opSEL_RB1)        { eatCycles(1); pswReg |= PSW_B; updateRegisters(); }

DO_OPC(opSTOP_TCNT)      { eatCycles(1);  }
DO_OPC(opSTOP_T)         { eatCycles(1);  }
DO_OPC(opSTRT_CNT)
{
    eatCycles(1);
}

DO_OPC(opSWAP_A)        { eatCycles(1); aReg = (aReg << 4) | (aReg >> 4); }

DO_OPC(opXCH_A_R0)      { eatCycles(1); uint8_t tmp = aReg; aReg = R0; R0 = tmp; }
DO_OPC(opXCH_A_R1)      { eatCycles(1); uint8_t tmp = aReg; aReg = R1; R1 = tmp; }
DO_OPC(opXCH_A_R2)      { eatCycles(1); uint8_t tmp = aReg; aReg = R2; R2 = tmp; }
DO_OPC(opXCH_A_R3)      { eatCycles(1); uint8_t tmp = aReg; aReg = R3; R3 = tmp; }
DO_OPC(opXCH_A_R4)      { eatCycles(1); uint8_t tmp = aReg; aReg = R4; R4 = tmp; }
DO_OPC(opXCH_A_R5)      { eatCycles(1); uint8_t tmp = aReg; aReg = R5; R5 = tmp; }
DO_OPC(opXCH_A_R6)      { eatCycles(1); uint8_t tmp = aReg; aReg = R6; R6 = tmp; }
DO_OPC(opXCH_A_R7)      { eatCycles(1); uint8_t tmp = aReg; aReg = R7; R7 = tmp; }
DO_OPC(opXCH_A_XR0)     { eatCycles(1); uint8_t tmp = aReg; aReg = read8d(R0); write8d(R0, tmp); }
DO_OPC(opXCH_A_XR1)     { eatCycles(1); uint8_t tmp = aReg; aReg = read8d(R1); write8d(R1, tmp); }

DO_OPC(opXCHD_A_XR0)    { eatCycles(1); uint8_t tmp = read8d(R0);
                          write8d(R0, (tmp & 0xF0) | (aReg & 0x0F));
                          aReg = (aReg & 0xF0) | (tmp & 0x0F); }

DO_OPC(opXCHD_A_XR1)    { eatCycles(1); uint8_t tmp = read8d(R1);
                          write8d(R1, (tmp & 0xF0) | (aReg & 0x0F));
                          aReg = (aReg & 0xF0) | (tmp & 0x0F); }

DO_OPC(opXRL_A_R0)      { eatCycles(1); aReg ^= R0; }
DO_OPC(opXRL_A_R1)      { eatCycles(1); aReg ^= R1; }
DO_OPC(opXRL_A_R2)      { eatCycles(1); aReg ^= R2; }
DO_OPC(opXRL_A_R3)      { eatCycles(1); aReg ^= R3; }
DO_OPC(opXRL_A_R4)      { eatCycles(1); aReg ^= R4; }
DO_OPC(opXRL_A_R5)      { eatCycles(1); aReg ^= R5; }
DO_OPC(opXRL_A_R6)      { eatCycles(1); aReg ^= R6; }
DO_OPC(opXRL_A_R7)      { eatCycles(1); aReg ^= R7; }
DO_OPC(opXRL_A_XR0)     { eatCycles(1); aReg ^= read8d(R0); }
DO_OPC(opXRL_A_XR1)     { eatCycles(1); aReg ^= read8d(R1); }
DO_OPC(opXRL_A_N)       { eatCycles(2); aReg ^= read8i(); }
