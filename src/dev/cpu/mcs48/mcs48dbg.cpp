// mcs48dbg.cpp - MCS 48 Processor series package - Debugging routines
//
// Author:  Tim Stark
// Date:    Jan 6, 2022

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "dev/cpu/mcs48/mcs48.h"

int mcs48_cpuDevice::list(offs_t vAddr)
{
	uint8_t  opCode, opMask;
	uint16_t reg;
	uint16_t addr;
	uint16_t sAddr = vAddr;
	std::string line, opcName, oprLine = "";

	opCode = mapProgram.read8(vAddr);
	vAddr = (vAddr + 1) & 0x7FF;

	line = fmt::sprintf("%03X %02X ", sAddr, opCode);
	if (opCodes[opCode] != nullptr)
	{
		opMask =  opCodes[opCode]->opMask;
		opcName = opCodes[opCode]->opName;
		oprLine = opCodes[opCode]->opReg;
		switch (opCodes[opCode]->opType)
		{
		case OPR_NOPE:
			oprLine = opCodes[opCode]->opReg;
			break;

		case OPR_REG:
			reg = opCode & 0x07;
			oprLine = fmt::sprintf(opCodes[opCode]->opReg, reg);
			break;

		case OPR_LIT:
			addr = mapProgram.read8(vAddr);
			vAddr = (vAddr + 1) & 0x7FF;
			line += fmt::sprintf("%02X", addr & 0xFF);
			oprLine = fmt::sprintf(opCodes[opCode]->opReg, addr);
			break;

		case OPR_REG|OPR_LIT:
			reg = opCode & 0x07;
			addr = mapProgram.read8(vAddr);
			vAddr = (vAddr + 1) & 0x7FF;
			line += fmt::sprintf("%02X", addr & 0xFF);
			oprLine = fmt::sprintf(opCodes[opCode]->opReg, reg, addr);
			break;

		case OPR_ADDR2:
			addr = mapProgram.read8(vAddr) | (vAddr & 0xF00);
			vAddr = (vAddr + 1) & 0x7FF;
			line += fmt::sprintf("%02X", addr & 0xFF);
			oprLine = fmt::sprintf(opCodes[opCode]->opReg, addr);
			break;

		case OPR_REG|OPR_ADDR2:
			reg = opCode & 0x07;
			addr = mapProgram.read8(vAddr) | (vAddr & 0xF00);
			vAddr = (vAddr + 1) & 0x7FF;
			line += fmt::sprintf("%02X", addr & 0xFF);
			oprLine = fmt::sprintf(opCodes[opCode]->opReg, reg, addr);
			break;

		case OPR_ADDR3:
			addr = ((opCode & 0xE0) << 3) | mapProgram.read8(vAddr);
			vAddr = (vAddr + 1) & 0x7FF;
			line += fmt::sprintf("%02X", addr & 0xFF);
			oprLine = fmt::sprintf(opCodes[opCode]->opReg, addr);
		}
	}
	else
		opcName = fmt::sprintf("<Unknown opcode = %02X>", opCode);

	fmt::printf("%-10s %-5s %s\n", line, opcName, oprLine);

	return vAddr - sAddr;
}

void mcs48_cpuDevice::initOpcodeTable()
{
	// Clear all opcode table
	for (int idx = 0; idx < 256; idx++)
		opCodes[idx] = nullptr;

	for (int idx = 0; opTable[idx].opName != nullptr; idx++)
	{
		if ((opTable[idx].opFlags & archFlags) == 0)
			continue;

		uint8_t opCode = opTable[idx].opCode;
		uint8_t opMask = opTable[idx].opMask;

		// fmt::printf("Opcode: %s\n", opTable[idx].opName);

		if (opMask == 0x00)
			opCodes[opCode] = &opTable[idx];
		else if (opMask == 0xE0)
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
		else for (int opIndex = opCode; opIndex < (opCode + opMask + 1); opIndex++)
		{
			opCodes[opIndex] = &opTable[idx];
			// fmt::printf("Name: %s Opcode: %02X\n", opTable[idx].opName, opIndex);
		}
	}
}
