// m68kcpu.h - Motora 68000 Processor Series Package
//
// Author:  Tim Stark
// Date:    Jun 16, 2023

#pragma once

class m68k_cpuDevice : public CPUDevice
{
public:


protected:
    uint32_t daRegs[16];    // Data/Address Registers
    uint32_t spRegs[7];     // Stack Pointer Registers

    map::MemoryAccess<24, 0, 0, BigEndian>::specific mapProgram8;
    map::MemoryAccess<24, 1, 0, BigEndian>::specific mapProgram16;
    map::MemoryAccess<32, 2, 0, BigEndian>::specific mapProgram32;

    // Memory initialization routines
    void init_8();
    void init_16();
    void init_32();
    void init_32pmmu();
    void init_32hmmu();

    // CPU initialization routines
    void init_common();
    void init_68000();
    void init_68008();
    void init_68010();
    void init_68020();
    void init_68020fpu();
    void init_68020pmmu();
    void init_68020hmmu();
    void init_68ec020();
    void init_68030();
    void init_68ec030();
    void init_68040();
    void init_68ec040();
    void init_68lc040();
    void init_coldfire();
};