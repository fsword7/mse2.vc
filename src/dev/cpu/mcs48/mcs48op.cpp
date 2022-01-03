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
    // Check IRQ for A11 bit address
    pcReg = addr | (irqInProgress ? 0 : a11Reg);
}

void mcs48_cpuDevice::exCALL(uint16_t addr)
{
    pushPCPSW();
    exJUMP(addr);
}

#define DO_OPC(name) void mcs48_cpuDevice::name()

DO_OPC(illegal)
{

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

DO_OPC(opCALL_0)        { eatCycles(2); exCALL(read8i() | 0x000); }
DO_OPC(opCALL_1)        { eatCycles(2); exCALL(read8i() | 0x100); }
DO_OPC(opCALL_2)        { eatCycles(2); exCALL(read8i() | 0x200); }
DO_OPC(opCALL_3)        { eatCycles(2); exCALL(read8i() | 0x300); }
DO_OPC(opCALL_4)        { eatCycles(2); exCALL(read8i() | 0x400); }
DO_OPC(opCALL_5)        { eatCycles(2); exCALL(read8i() | 0x500); }
DO_OPC(opCALL_6)        { eatCycles(2); exCALL(read8i() | 0x600); }
DO_OPC(opCALL_7)        { eatCycles(2); exCALL(read8i() | 0x700); }

