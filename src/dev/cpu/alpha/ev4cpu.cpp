// ev4cpu.cpp - Alpha EV4 Procssor package (21064 series)
//
// Author:  Tim Stark
// Date:    May 21, 2023

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "emu/devcpu.h"
#include "dev/cpu/alpha/axpcpu.h"
#include "dev/cpu/alpha/ev4cpu.h"
#include "dev/cpu/alpha/ev4ipr.h"

EV4Processor::EV4Processor(SystemConfig &config, cDeviceType &type,
    cstr_t &devName, Device *owner, uint64_t clock)
: AlphaProcessor(config, type, devName, owner, clock, 34, 9)
{
}

void EV4Processor::pal_call()
{
    int func = OP_GETPAL(axp.opWord);
    uint64_t vAddr;

    if (((func < 0x40) && (axp.icm == ACC_KERNEL)) ||
        ((func >= 0x80) && (func < 0xC0)))
    {
		// Save current PC address for HW_REI instruction
        axp.excAddr = axp.vpcAddr;
		vAddr = axp.palBase | (1u << 13) | PC_PAL_MODE |
			((func & 0x80) << 5) | ((func & 0x3F) << 6);
		setPC(vAddr);
    }

    // OPCDEC exception trap
}

void EV4Processor::hw_ld()
{

}

void EV4Processor::hw_st()
{

}

void EV4Processor::hw_rei()
{
    setPC(axp.excAddr);
}

void EV4Processor::hw_mtpr()
{
    int index = axp.opWord & 0xFF;

    switch(index)
    {
    // IBX registers
    case IPR4n_EXC_ADDR:
        axp.excAddr = RBV;
        break;

    case IPR4n_PAL_BASE:
        axp.palBase = RBV;
        break;

    case IPR4n_PS:
        axp.icm = (RBV >> 3) & 0x3;
        break;

    case IPR4n_ALT_MODE:
        axp.altcm = (RBV >> 3) & 0x3;
        break;

    // PAL registers
    case IPR4n_PAL_TEMP0:  case IPR4n_PAL_TEMP1:  case IPR4n_PAL_TEMP2:  case IPR4n_PAL_TEMP3:
    case IPR4n_PAL_TEMP4:  case IPR4n_PAL_TEMP5:  case IPR4n_PAL_TEMP6:  case IPR4n_PAL_TEMP7:
    case IPR4n_PAL_TEMP8:  case IPR4n_PAL_TEMP9:  case IPR4n_PAL_TEMP10: case IPR4n_PAL_TEMP11:
    case IPR4n_PAL_TEMP12: case IPR4n_PAL_TEMP13: case IPR4n_PAL_TEMP14: case IPR4n_PAL_TEMP15:
    case IPR4n_PAL_TEMP16: case IPR4n_PAL_TEMP17: case IPR4n_PAL_TEMP18: case IPR4n_PAL_TEMP19:
    case IPR4n_PAL_TEMP20: case IPR4n_PAL_TEMP21: case IPR4n_PAL_TEMP22: case IPR4n_PAL_TEMP23:
    case IPR4n_PAL_TEMP24: case IPR4n_PAL_TEMP25: case IPR4n_PAL_TEMP26: case IPR4n_PAL_TEMP27:
    case IPR4n_PAL_TEMP28: case IPR4n_PAL_TEMP29: case IPR4n_PAL_TEMP30: case IPR4n_PAL_TEMP31:
        ev4.palTemp[index & 0x1F] = RBV;
        break;
    }
}

void EV4Processor::hw_mfpr()
{
    int index = axp.opWord & 0xFF;

    switch(index)
    {
    // IBX registers
    case IPR4n_EXC_ADDR:
        RAV = axp.excAddr;
        break;

    case IPR4n_PAL_BASE:
        RAV = axp.palBase;
        break;

    case IPR4n_PS:
        RAV = uint64_t(axp.icm & 2) << 33 | uint64_t(axp.icm & 1) << 1;
        break;

    // PAL registers
    case IPR4n_PAL_TEMP0:  case IPR4n_PAL_TEMP1:  case IPR4n_PAL_TEMP2:  case IPR4n_PAL_TEMP3:
    case IPR4n_PAL_TEMP4:  case IPR4n_PAL_TEMP5:  case IPR4n_PAL_TEMP6:  case IPR4n_PAL_TEMP7:
    case IPR4n_PAL_TEMP8:  case IPR4n_PAL_TEMP9:  case IPR4n_PAL_TEMP10: case IPR4n_PAL_TEMP11:
    case IPR4n_PAL_TEMP12: case IPR4n_PAL_TEMP13: case IPR4n_PAL_TEMP14: case IPR4n_PAL_TEMP15:
    case IPR4n_PAL_TEMP16: case IPR4n_PAL_TEMP17: case IPR4n_PAL_TEMP18: case IPR4n_PAL_TEMP19:
    case IPR4n_PAL_TEMP20: case IPR4n_PAL_TEMP21: case IPR4n_PAL_TEMP22: case IPR4n_PAL_TEMP23:
    case IPR4n_PAL_TEMP24: case IPR4n_PAL_TEMP25: case IPR4n_PAL_TEMP26: case IPR4n_PAL_TEMP27:
    case IPR4n_PAL_TEMP28: case IPR4n_PAL_TEMP29: case IPR4n_PAL_TEMP30: case IPR4n_PAL_TEMP31:
        RAV = ev4.palTemp[index & 0x1F];
        break;
    }
}

DEFINE_DEVICE_TYPE(AXP21064, axp21064_cpuDevice, "AXP 21064", "DEC Alpha 21064")

axp21064_cpuDevice::axp21064_cpuDevice(SystemConfig &config, cstr_t &devName, Device *owner, uint64_t clock)
: EV4Processor(config, AXP21064, devName, owner, clock)
{ }
