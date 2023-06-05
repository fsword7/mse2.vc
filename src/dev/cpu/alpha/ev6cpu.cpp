// ev6cpu.cpp - Alpha EV6 Procssor package (21264 series)
//
// Author:  Tim Stark
// Date:    May 21, 2023

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "emu/devcpu.h"
#include "dev/cpu/alpha/axpcpu.h"
#include "dev/cpu/alpha/ev6cpu.h"
#include "dev/cpu/alpha/ev6ipr.h"

EV6Processor::EV6Processor(SystemConfig &config, cDeviceType &type,
    cstr_t &devName, Device *owner, uint64_t clock)
: AlphaProcessor(config, type, devName, owner, clock, 44, 11)
{
}

void EV6Processor::pal_call()
{
    int func = OP_GETPAL(axp.opWord);
    uint64_t vAddr;

    if (((func < 0x40) && (axp.icm == ACC_KERNEL)) ||
        ((func >= 0x80) && (func < 0xC0)))
    {
		// Save current PC address for HW_RET instruction
		axp.gRegs[32 + 23] = axp.vpcAddr;
		vAddr = axp.palBase | (1u << 13) | PC_PAL_MODE |
			((func & 0x80) << 5) | ((func & 0x3F) << 6);
		setPC(vAddr);
    }

    // OPCDEC exception trap
}

void EV6Processor::hw_ld()
{

}

void EV6Processor::hw_st()
{

}

void EV6Processor::hw_rei()
{
    setPC(axp.excAddr);
}

void EV6Processor::hw_mtpr()
{
    int mask  = axp.opWord & 0xFF;
    int index = (axp.opWord >> 8) & 0xFF;

    switch(index)
    {
    case IPR6n_PAL_BASE:
        axp.palBase = RBV;
    
    case IPR6n_CM:
        axp.icm = (RBV >> 3) & 3;
        break;

    case IPR6n_DTB_ATLMODE:
        axp.altcm = RBV & 3;
        break;

    // case IPR6n_DTB_CM:
    //     axp.dcm = RBV;
    //     break;

    // PAL registers
    // case IPR6n_PAL_TEMP0:  case IPR6n_PAL_TEMP1:  case IPR6n_PAL_TEMP2:  case IPR6n_PAL_TEMP3:
    // case IPR6n_PAL_TEMP4:  case IPR6n_PAL_TEMP5:  case IPR6n_PAL_TEMP6:  case IPR6n_PAL_TEMP7:
    // case IPR6n_PAL_TEMP8:  case IPR6n_PAL_TEMP9:  case IPR6n_PAL_TEMP10: case IPR6n_PAL_TEMP11:
    // case IPR6n_PAL_TEMP12: case IPR6n_PAL_TEMP13: case IPR6n_PAL_TEMP14: case IPR6n_PAL_TEMP15:
    // case IPR6n_PAL_TEMP16: case IPR6n_PAL_TEMP17: case IPR6n_PAL_TEMP18: case IPR6n_PAL_TEMP19:
    // case IPR6n_PAL_TEMP20: case IPR6n_PAL_TEMP21: case IPR6n_PAL_TEMP22: case IPR6n_PAL_TEMP23:
    //     ev6.palTemp[index & 0x1F] = RBV;
        break;
    }
}

void EV6Processor::hw_mfpr()
{
    int mask  = axp.opWord & 0xFF;
    int index = (axp.opWord >> 8) & 0xFF;

    switch(index)
    {
    // IDU registers
    case IPR6n_EXC_ADDR:
        RAV = axp.excAddr;
        break;

    case IPR6n_PAL_BASE:
        RAV = axp.palBase;
        break;

    case IPR6n_CM:
        RAV = axp.icm << 3;
        break;

    // // MTU registers
    // case IPR6n_DTB_CM:
    //     RAV = axp.dcm;
    //     break;

    // PAL registers
    // case IPR6n_PAL_TEMP0:  case IPR6n_PAL_TEMP1:  case IPR6n_PAL_TEMP2:  case IPR6n_PAL_TEMP3:
    // case IPR6n_PAL_TEMP4:  case IPR6n_PAL_TEMP5:  case IPR6n_PAL_TEMP6:  case IPR6n_PAL_TEMP7:
    // case IPR6n_PAL_TEMP8:  case IPR6n_PAL_TEMP9:  case IPR6n_PAL_TEMP10: case IPR6n_PAL_TEMP11:
    // case IPR6n_PAL_TEMP12: case IPR6n_PAL_TEMP13: case IPR6n_PAL_TEMP14: case IPR6n_PAL_TEMP15:
    // case IPR6n_PAL_TEMP16: case IPR6n_PAL_TEMP17: case IPR6n_PAL_TEMP18: case IPR6n_PAL_TEMP19:
    // case IPR6n_PAL_TEMP20: case IPR6n_PAL_TEMP21: case IPR6n_PAL_TEMP22: case IPR6n_PAL_TEMP23:
    //     RAV = ev6.palTemp[index & 0x1F];
    //     break;
    }
}

DEFINE_DEVICE_TYPE(AXP21264, axp21264_cpuDevice, "AXP 21264", "DEC Alpha 21264")

axp21264_cpuDevice::axp21264_cpuDevice(SystemConfig &config, cstr_t &devName, Device *owner, uint64_t clock)
: EV6Processor(config, AXP21264, devName, owner, clock)
{ }
