// ev5cpu.cpp - Alpha EV5 Procssor package (21164 series)
//
// Author:  Tim Stark
// Date:    May 21, 2023

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "emu/devcpu.h"
#include "dev/cpu/alpha/axpcpu.h"
#include "dev/cpu/alpha/ev5cpu.h"
#include "dev/cpu/alpha/ev5ipr.h"

EV5Processor::EV5Processor(const SystemConfig &config, cDeviceType &type,
    cstr_t &devName, Device *owner, uint64_t clock)
: AlphaProcessor(config, type, devName, owner, clock, 40, 10)
{
}

void EV5Processor::pal_call()
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

void EV5Processor::hw_ld()
{

}

void EV5Processor::hw_st()
{

}

void EV5Processor::hw_rei()
{
    setPC(axp.excAddr);
}

void EV5Processor::hw_mtpr()
{
    int index = axp.opWord & 0xFF;

    switch(index)
    {
    // IDU registers
    case IPR5n_EXC_ADDR:
        axp.excAddr = RBV;
        break;

    case IPR5n_PAL_BASE:
        axp.palBase = RBV;
        break;

    case IPR5n_ICM:
        axp.icm = RBV;
        break;

    // MTU registers
    case IPR5n_DTB_CM:
        axp.dcm = RBV;
        break;

    case IPR5n_ALT_MODE:
        axp.altcm = RBV;
        break;

    // PAL registers
    case IPR5n_PAL_TEMP0:  case IPR5n_PAL_TEMP1:  case IPR5n_PAL_TEMP2:  case IPR5n_PAL_TEMP3:
    case IPR5n_PAL_TEMP4:  case IPR5n_PAL_TEMP5:  case IPR5n_PAL_TEMP6:  case IPR5n_PAL_TEMP7:
    case IPR5n_PAL_TEMP8:  case IPR5n_PAL_TEMP9:  case IPR5n_PAL_TEMP10: case IPR5n_PAL_TEMP11:
    case IPR5n_PAL_TEMP12: case IPR5n_PAL_TEMP13: case IPR5n_PAL_TEMP14: case IPR5n_PAL_TEMP15:
    case IPR5n_PAL_TEMP16: case IPR5n_PAL_TEMP17: case IPR5n_PAL_TEMP18: case IPR5n_PAL_TEMP19:
    case IPR5n_PAL_TEMP20: case IPR5n_PAL_TEMP21: case IPR5n_PAL_TEMP22: case IPR5n_PAL_TEMP23:
        ev5.palTemp[index & 0x1F] = RBV;
        break;
    }
}

void EV5Processor::hw_mfpr()
{
    int index = axp.opWord & 0xFF;

    switch(index)
    {
    // IDU registers
    case IPR5n_EXC_ADDR:
        RAV = axp.excAddr;
        break;

    case IPR5n_PAL_BASE:
        RAV = axp.palBase;
        break;

    case IPR5n_ICM:
        RAV = axp.icm;
        break;

    // MTU registers
    case IPR5n_DTB_CM:
        RAV = axp.dcm;
        break;

    case IPR5n_ALT_MODE:
        RAV = axp.altcm;
        break;

    // PAL registers
    case IPR5n_PAL_TEMP0:  case IPR5n_PAL_TEMP1:  case IPR5n_PAL_TEMP2:  case IPR5n_PAL_TEMP3:
    case IPR5n_PAL_TEMP4:  case IPR5n_PAL_TEMP5:  case IPR5n_PAL_TEMP6:  case IPR5n_PAL_TEMP7:
    case IPR5n_PAL_TEMP8:  case IPR5n_PAL_TEMP9:  case IPR5n_PAL_TEMP10: case IPR5n_PAL_TEMP11:
    case IPR5n_PAL_TEMP12: case IPR5n_PAL_TEMP13: case IPR5n_PAL_TEMP14: case IPR5n_PAL_TEMP15:
    case IPR5n_PAL_TEMP16: case IPR5n_PAL_TEMP17: case IPR5n_PAL_TEMP18: case IPR5n_PAL_TEMP19:
    case IPR5n_PAL_TEMP20: case IPR5n_PAL_TEMP21: case IPR5n_PAL_TEMP22: case IPR5n_PAL_TEMP23:
        RAV = ev5.palTemp[index & 0x1F];
        break;
    }
}

// DEFINE_DEVICE_TYPE(AXP21064, axp21064_cpuDevice, "AXP 21064", "DEC Alpha 21064")

// axp21064_cpuDevice::axp21064_cpuDevice(const SystemConfig &config, cstr_t &devName, Device *owner, uint64_t clock)
// : EV4Processor(config, AXP21064, devName, owner, clock)
// { }
