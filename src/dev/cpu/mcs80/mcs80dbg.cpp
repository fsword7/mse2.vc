// mcs80dbg.cpp - Intel 8080/8085 Processor package - debugging tools
// Author:  Tim Stark
// Date:    Jan 10, 2022

#include "emu/core.h"
#include "emu/map/map.h"
#include "dev/cpu/mcs80/mcs80.h"
#include "dev/cpu/mcs80/mcs80dbg.h"

int mcs80_cpuDevice::list(offs_t vAddr)
{
	uint8_t  opCode, opMask;
	uint16_t rs, rd, val;
	uint16_t addr;
	uint16_t sAddr = vAddr;
	std::string line, opcName, oprLine = "";

	opCode = mapProgram.read8(vAddr);
	vAddr = (vAddr + 1) & 0xFFFF;

	line = fmt::sprintf("%04X %02X ", sAddr, opCode);
	if (opCodes[opCode] != nullptr)
	{
		opMask =  opCodes[opCode]->opMask;
		opcName = opCodes[opCode]->opName;
		switch (opCodes[opCode]->opType)
		{
		case OPR_IMPLIED:
			// oprLine = opCodes[opCode]->opReg;
			break;

		case OPR_RS8:
			rs = opCode & 0x07;
			oprLine = fmt::sprintf("%s", regList8[rs]);
			break;

		case OPR_RD8:
			rd = (opCode >> 3) & 0x07;
			oprLine = fmt::sprintf("%s", regList8[rd]);
			break;

		case OPR_RSD8:
			rs = opCode & 0x07;
            rd = (opCode >> 3) & 0x07;
			oprLine = fmt::sprintf("%s,%s", regList8[rd], regList8[rs]);
			break;

		case OPR_REG16S:
			rs = (opCode >> 4) & 0x03;
			oprLine = fmt::sprintf("%s", regList16s[rs]);
			break;

		case OPR_REG16P:
			rs = (opCode >> 4) & 0x03;
			oprLine = fmt::sprintf("%s", regList16p[rs]);
			break;

		case OPR_REGI8:
			rd = (opCode >> 3) & 0x07;
			val = mapProgram.read8(vAddr);
			vAddr = (vAddr + 1) & 0xFFFF;
			line += fmt::sprintf("%02X", val & 0xFF);
			oprLine = fmt::sprintf("%s,#%02X", regList8[rd], val);
			break;

		case OPR_REGI16:
			rs = (opCode >> 4) & 0x03;
			val = mapProgram.read8(vAddr) | (mapProgram.read8(vAddr+1) << 8);
			vAddr = (vAddr + 2) & 0xFFFF;
			line += fmt::sprintf("%02X %02X", val & 0xFF, (val >> 8) & 0xFF);
			oprLine = fmt::sprintf("%s,#%04X", regList16s[rs], val);
			break;

		case OPR_IMM8:
			val = mapProgram.read8(vAddr);
			vAddr = (vAddr + 1) & 0xFFFF;
			line += fmt::sprintf("%02X", val & 0xFF);
			oprLine = fmt::sprintf("#%02X", val);
			break;

		case OPR_PORT:
			val = mapProgram.read8(vAddr);
			vAddr = (vAddr + 1) & 0xFFFF;
			line += fmt::sprintf("%02X", val & 0xFF);
			oprLine = fmt::sprintf("%02X", val);
			break;

		case OPR_VECTOR:
			val = mapProgram.read8(vAddr);
			vAddr = (vAddr + 1) & 0xFFFF;
			line += fmt::sprintf("%02X", val & 0xFF);
			oprLine = fmt::sprintf("%d", val);
			break;

		case OPR_ADDR:
			addr = mapProgram.read8(vAddr) | (mapProgram.read8(vAddr+1) << 8);
			vAddr = (vAddr + 2) & 0xFFFF;
			line += fmt::sprintf("%02X %02X", addr & 0xFF, (addr >> 8) & 0xFF);
			oprLine = fmt::sprintf("%04X", addr);
			break;
		}
	}
	else
		opcName = fmt::sprintf("<Unknown opcode = %02X>", opCode);

	fmt::printf("%-14s %-5s %s\n", line, opcName, oprLine);

	return vAddr - sAddr;
}

void mcs80_cpuDevice::initOpcodeTable()
{
	// Clear all opcode table
	for (int idx = 0; idx < 256; idx++)
		opCodes[idx] = nullptr;

	for (int idx = 0; opTable[idx].opName != nullptr; idx++)
	{
		// if ((opTable[idx].opFlags & archFlags) == 0)
		// 	continue;

		uint8_t opCode = opTable[idx].opCode;
		uint8_t opMask = opTable[idx].opMask;

		// fmt::printf("Opcode: %s\n", opTable[idx].opName);

		if (opMask == 0x00)
			opCodes[opCode] = &opTable[idx];
		else
		{
			uint8_t bit = 0;
			while(((opMask >> bit) & 1) == 0)
				bit++;
			int count = (opMask >> bit) + 1;
			for (int opIndex = opCode; count; opIndex += (1 << bit), count--)
			{
				opCodes[opIndex] = &opTable[idx];
				// fmt::printf("Name: %s Opcode: %02X\n", opTable[idx].opName, opIndex);
			}
		}
		// else for (int opIndex = opCode; opIndex < (opCode + opMask + 1); opIndex++)
		// {
		// 	opCodes[opIndex] = &opTable[idx];
		// 	// fmt::printf("Name: %s Opcode: %02X\n", opTable[idx].opName, opIndex);
		// }
	}
}
