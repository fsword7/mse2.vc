// execute.cpp - 8080/8080A/8085 processor - execute routines
//
// Author:  Tim Stark
// Date:    12/23/21

#include "emu/core.h"
#include "emu/map/map.h"
#include "dev/cpu/i8080/i8080.h"

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

void i8080_cpuDevice::init()
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

uint8_t i8080_cpuDevice::readi8()
{
    return mapProgram.read8(pcReg.uw++);
}

pair16_t i8080_cpuDevice::readi16()
{
    pair16_t w;

    w.ub.l = mapProgram.read8(pcReg.uw++);
    w.ub.h = mapProgram.read8(pcReg.uw++);

    return w;
}

uint8_t i8080_cpuDevice::read8(offs_t addr)
{
    return mapProgram.read8(addr);
}

void i8080_cpuDevice::write8(offs_t addr, uint8_t data)
{
    mapProgram.write8(addr, data);
}


void i8080_cpuDevice::execute()
{
    uint8_t opCode;

    pcBase = pcReg.uw;
    opCode = readi8();

    // Takes CPU cycles each instruction
    opCount =- opCycleTable[opCode];

    switch (opCode)
    {

    }
}