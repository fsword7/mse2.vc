// m68kcpu.cpp - m68k Processor Series Package
//
// Author:  Tim Stark
// Date:    Jun 16, 2023

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "emu/devcpu.h"
#include "dev/cpu/m68k/m68kcpu.h"


void m68k_cpuDevice::init_8()
{

}

void m68k_cpuDevice::init_16()
{

}

void m68k_cpuDevice::init_32()
{

}

void m68k_cpuDevice::init_32pmmu()
{

}

void m68k_cpuDevice::init_32hmmu()
{

}


// CPU initialization routines

void m68k_cpuDevice::init_common()
{

}

void m68k_cpuDevice::init_68000()
{
    init_16();
}

void m68k_cpuDevice::init_68008()
{
    init_8();
}

void m68k_cpuDevice::init_68010()
{
    init_16();
}

void m68k_cpuDevice::init_68020()
{
    init_32();
}

void m68k_cpuDevice::init_68020fpu()
{
    init_32();
}

void m68k_cpuDevice::init_68020pmmu()
{
    init_32pmmu();
}

void m68k_cpuDevice::init_68020hmmu()
{
    init_32hmmu();
}

void m68k_cpuDevice::init_68ec020()
{
    init_32();
}

void m68k_cpuDevice::init_68030()
{
    init_32pmmu();
}

void m68k_cpuDevice::init_68ec030()
{
    init_32();
}

void m68k_cpuDevice::init_68040()
{
    init_32pmmu();
}

void m68k_cpuDevice::init_68ec040()
{
    init_32();
}

void m68k_cpuDevice::init_68lc040()
{
    init_32pmmu();
}

void m68k_cpuDevice::init_coldfire()
{
    init_32();
}