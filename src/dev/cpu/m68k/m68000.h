// m68000.h - m68k - 68000 Processor Package
//
// Author:  Tim Stark
// Date:    Jun 16, 2023

#pragma once

class m68000_cpuDevice
{


protected:
    uint32_t daRegs[17];    // D0-D7 / A0-A6 / USP / SSP Registers



    map::MemoryAccess<24, 1, 0, BigEndian>::specific mapuProgram;
    map::MemoryAccess<24, 1, 0, BigEndian>::specific mapsProgram;

};