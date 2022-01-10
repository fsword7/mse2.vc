// mcs80op.cpp - Intel 8080/8085 Processor package - opcode tables
//
// Author:  Tim Stark
// Date:    Jan 9, 2022

#include "emu/core.h"
#include "emu/map/map.h"
#include "dev/cpu/mcs80/mcs80.h"

#define INST(name, opcode, opmask, optype) \
    { name, opcode, opmask, optype }

const mcs80_opCode_t mcs80_cpuDevice::opTable[] =
{
    INST("MOV",     0x40, 0x3F, OPR_RSD8 ),
    INST("MVI",     0x06, 0x38, OPR_REGI8),
    INST("LXI",     0x01, 0x30, OPR_REGI16),
    INST("LDA",     0x3A, 0x00, OPR_ADDR),
    INST("STA",     0x32, 0x00, OPR_ADDR),
    INST("LHLD",    0x2A, 0x00, OPR_ADDR),
    INST("SHLD",    0x22, 0x00, OPR_ADDR),
    INST("LDAX",    0x0A, 0x30, OPR_REG16S),
    INST("STAX",    0x02, 0x30, OPR_REG16S),
    INST("XCHG",    0x7B, 0x00, OPR_IMPLIED),

    INST("ADD",     0x80, 0x07, OPR_RS8),
    INST("ADI",     0xC6, 0x00, OPR_IMM8),
    INST("ADC",     0x88, 0x07, OPR_RS8),
    INST("ACI",     0xC7, 0x00, OPR_IMM8),
    INST("SUB",     0x90, 0x07, OPR_RS8),
    INST("SUI",     0xD6, 0x00, OPR_IMM8),
    INST("SBB",     0x98, 0x07, OPR_RS8),
    INST("SBI",     0xDE, 0x00, OPR_IMM8),
    INST("INR",     0x04, 0x38, OPR_RD8),
    INST("DCR",     0x05, 0x38, OPR_RD8),
    INST("INX",     0x03, 0x30, OPR_REG16S),
    INST("DCX",     0x0B, 0x30, OPR_REG16S),
    INST("DAD",     0x09, 0x30, OPR_REG16S),
    INST("DAA",     0x27, 0x00, OPR_IMPLIED),

    INST("ANA",     0xA0, 0x07, OPR_RS8),
    INST("ANI",     0xE6, 0x00, OPR_IMM8),
    INST("XRA",     0xA8, 0x07, OPR_RS8),
    INST("XRI",     0xEE, 0x00, OPR_IMM8),
    INST("ORA",     0xB0, 0x07, OPR_RS8),
    INST("ORI",     0xF6, 0x00, OPR_IMM8),
    INST("CMP",     0xB8, 0x07, OPR_RS8),
    INST("CPI",     0xFE, 0x00, OPR_IMM8),
    INST("RLC",     0x07, 0x00, OPR_IMPLIED),
    INST("RRC",     0x0F, 0x00, OPR_IMPLIED),
    INST("RAL",     0x17, 0x00, OPR_IMPLIED),
    INST("RAR",     0x1F, 0x00, OPR_IMPLIED),
    INST("CMA",     0x2F, 0x00, OPR_IMPLIED),
    INST("CMC",     0x3F, 0x00, OPR_IMPLIED),
    INST("STC",     0x37, 0x00, OPR_IMPLIED),

    INST("JMP",     0xC3, 0x00, OPR_ADDR),
    INST("JNZ",     0xC2, 0x00, OPR_ADDR),
    INST("JZ",      0xCA, 0x00, OPR_ADDR),
    INST("JNC",     0xD2, 0x00, OPR_ADDR),
    INST("JC",      0xDA, 0x00, OPR_ADDR),
    INST("JPO",     0xE2, 0x00, OPR_ADDR),
    INST("JPE",     0xEA, 0x00, OPR_ADDR),
    INST("JP",      0xF2, 0x00, OPR_ADDR),
    INST("JM",      0xFA, 0x00, OPR_ADDR),

    INST("CALL",    0xCD, 0x00, OPR_ADDR),
    INST("CNZ",     0xC4, 0x00, OPR_ADDR),
    INST("CZ",      0xCC, 0x00, OPR_ADDR),
    INST("CNC",     0xD4, 0x00, OPR_ADDR),
    INST("CC",      0xDC, 0x00, OPR_ADDR),
    INST("CPO",     0xE4, 0x00, OPR_ADDR),
    INST("CPE",     0xEC, 0x00, OPR_ADDR),
    INST("CP",      0xF4, 0x00, OPR_ADDR),
    INST("CM",      0xFC, 0x00, OPR_ADDR),

    INST("RET",     0xC9, 0x00, OPR_IMPLIED),
    INST("RNZ",     0xC0, 0x00, OPR_IMPLIED),
    INST("RZ",      0xC8, 0x00, OPR_IMPLIED),
    INST("RNC",     0xD0, 0x00, OPR_IMPLIED),
    INST("RC",      0xD8, 0x00, OPR_IMPLIED),
    INST("RPO",     0xE0, 0x00, OPR_IMPLIED),
    INST("RPE",     0xE8, 0x00, OPR_IMPLIED),
    INST("RP",      0xF0, 0x00, OPR_IMPLIED),
    INST("RM",      0xF8, 0x00, OPR_IMPLIED),

    INST("RST",     0xC7, 0x38, OPR_VECTOR),
    INST("PCHL",    0xE9, 0x00, OPR_IMPLIED),

    INST("PUSH",    0xC5, 0x30, OPR_REG16P),
    INST("POP",     0xC1, 0x30, OPR_REG16P),
    INST("XTHL",    0xE3, 0x00, OPR_IMPLIED),
    INST("SPHL",    0xF9, 0x00, OPR_IMPLIED),
    INST("IN",      0xDB, 0x00, OPR_PORT),
    INST("OUT",     0xD3, 0x00, OPR_PORT),

    INST("EI",      0xFB, 0x00, OPR_IMPLIED),
    INST("DI",      0xF3, 0x00, OPR_IMPLIED),
    INST("HLT",     0x76, 0x00, OPR_IMPLIED),
    INST("NOP",     0x00, 0x00, OPR_IMPLIED),
    INST("RIM",     0x20, 0x00, OPR_IMPLIED),
    INST("SIM",     0x30, 0x00, OPR_IMPLIED),

    // Terminator
    nullptr
};



