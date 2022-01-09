// upi41.cpp - UPI-41 microcontroller series package
//
// Author:  Tim Stark
// Date:    Jan 9, 2022

#include "emu/core.h"
#include "emu/devcb.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "dev/cpu/mcs48/mcs48.h"

// DBB read/write access function calls for UPI41 microcontroller

uint8_t upi41_cpuDevice::read8dbb(offs_t offset)
{
    if ((offset & 1) != 0)
        return (stsReg & 0xF3) | (f1Reg ? 8 : 0) | ((pswReg & PSW_F) ? 4 : 0);

    if (stsReg & STS_OBF)
    {
        stsReg &= ~STS_OBF;
        if (flagsEnable)
            write8port(2, p2Reg &= ~P2_OBF);
    }
    return dbboReg;
}

void upi41_cpuDevice::write8dbb(offs_t offset, uint8_t data)
{
    dbbiReg = data;
    if ((stsReg & STS_IBF) == 0)
    {
        stsReg |= STS_IBF;
        if (flagsEnable)
            write8port(2, p2Reg &= ~P2_NIBF);
    }

    f1Reg = offset & 1; 
}
