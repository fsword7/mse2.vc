// mcs48op.cpp - MCS 48 Processor series package - Opcode table
//
// Author:  Tim Stark
// Date:    1/3/22

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "dev/cpu/mcs48/mcs48.h"

#define OP(name) &mcs48_cpuDevice::op##name

const mcs48_cpuDevice::opExecute mcs48_cpuDevice::mcs48_Opcodes[] =
{
    OP(NOP),       OP(Illegal), OP(OUTL_BUS_A), OP(ADD_A_N), OP(JMP_0),     OP(EN_I),      OP(Illegal),   OP(DEC_A),
    OP(INS_A_BUS), OP(IN_A_P1), OP(IN_A_P2),    OP(Illegal), OP(MOVD_A_P4), OP(MOVD_A_P5), OP(MOVD_A_P6), OP(MOVD_A_P7), 
};