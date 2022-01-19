// upd7810dbg.cpp - NEC uPD7810 processor package - debugging routines
//
// Author:  Tim Stark
// Date:    Jan 17, 2022

#include "emu/core.h"
#include "dev/cpu/upd7810/upd7810.h"

static cchar_t *tokName[] =
{
        "", "Illegal",
        "ACI",    "ADC",    "ADCW",   "ADCX",   "ADD",    "ADDNC",  "ADDNCW", "ADDNCX",
        "ADDW",   "ADDX",   "ADI",    "ADINC",  "ANA",    "ANAW",   "ANAX",   "AND",
        "ANI",    "ANIW",   "BIT",    "BLOCK",  "CALB",   "CALF",   "CALL",   "CALT",
        "CLC",    "CLR",    "CMC",    "DAA",    "DADC",   "DADD",   "DADDNC", "DAN",
        "DCR",    "DCRW",   "DCRX",   "DCX",    "DEQ",    "DGT",    "DI",     "DIV",    
        "DLT",    "DMOV",   "DNE",    "DOFF",   "DON",    "DOR",    "DRLL",   "DRLR",
        "DSBB",   "DSLL",   "DSLR",   "DSUB",   "DSUBNB", "DXR",    "EADD",   "EI",
        "EQA",    "EQAW",   "EQAX",   "EQI",    "EQIW",   "ESUB",   "EX",     "EXA",
        "EXH",    "EXX",    "EXR",    "GTA",    "GTAW",   "GTAX",   "GTI",    "GTIW",
        "HALT",   "IN",     "INR",    "INRW",   "INX",    "JB",     "JEA",    "JMP",
        "JR",     "JRE",    "LBCD",   "LDAW",   "LDAX",   "LDEAX",  "LDED",   "LHLD",
        "LSPD",   "LTA",    "LTAW",   "LTAX",   "LTI",    "LTIW",   "LXI",    "MOV",
        "MUL",    "MVI",    "MVIW",   "MVIX",   "NEA",    "NEAW",   "NEAX",   "NEGA",
        "NEI",    "NEIW",   "NOP",    "NOT",    "OFFA",   "OFFAW",  "OFFAX",  "OFFI",
        "OFFIW",  "ONA",    "ONAW",   "ONAX",   "ONI",    "ONIW",   "OR",     "ORA",
        "ORAW",   "ORAX",   "ORI",    "ORIW",   "OUT",    "PER",    "PEX",    "POP",
        "PUSH",   "RET",    "RETI",   "RETS",   "RLD",    "RLL",    "RLR",    "RRD",
        "SBB",    "SBBW",   "SBBX",   "SBCD",   "SBI",    "SDED",   "SETB",   "SHLD",
        "SID",    "SK",     "SKIT",   "SKN",    "SKNIT",  "SLL",    "SLLC",   "SLR",
        "SLRC",   "SOFTI",  "SSPD",   "STAW",   "STAX",   "STC",    "STEAX",  "STM",
        "STOP",   "SUB",    "SUBNB",  "SUBNBW", "SUBNBX", "SUBW",   "SUBX",   "SUI",
        "SUINB",  "TABLE",  "XOR",    "XRA",    "XRAW",   "XRAX",   "XRI"
};

static cchar_t *regName[] =
{
    "Illegal", "Illegal", "Illegal", "Illegal",
    "Illegal", "Illegal", "Illegal", "Illegal",
    "Illegal", "Illegal", "Illegal", "Illegal",
    "Illegal", "Illegal", "Illegal", "Illegal",
    "PA",      "PB",      "PC",      "PD",
    "Illegal", "PF",      "MXH",     "MXL", 
    "Illegal", "SMH",     "Illegal", "EOM",
    "Illegal", "TMM",     "PT",      "Illegal"
};

int upd7810_cpuDevice::list(offs_t vAddr)
{
    offs_t addr = vAddr;
    uint8_t opCode = mapProgram.read8(addr++);
    uint8_t opField = 0;
    const dopTable_t *opInfo = &dopCode[opCode];

    std::string opcLine = "";
    std::string oprLine = "";
    uint16_t val, ea;

    opcLine = fmt::sprintf("%04X %02X", vAddr, opCode);
    if (opInfo->isPrefix())
    {
        opField = opCode;
        opCode = mapProgram.read8(addr++);
        opInfo = &opInfo->getPrefix()[opCode];
        opcLine += fmt::sprintf(" %02X", opCode);
    }
    
    for (cchar_t *ch = opInfo->getOperand(); ch != nullptr && *ch != '\0'; ch++)
    {
        if (*ch != '%')
        {
            oprLine += *ch;
            continue;
        }

        ch++;
        switch (*ch)
        {
        case 'b': // byte immediate
            val = mapProgram.read8(addr++);
            oprLine += fmt::sprintf("$%02X", val);
            opcLine += fmt::sprintf(" %02X", val);
            break;

        case 'w': // word immediate
            val = mapProgram.read8(addr++) |
                 (mapProgram.read8(addr++) << 8);
            oprLine += fmt::sprintf("$%04X", val);
            opcLine += fmt::sprintf(" %02X %02X", val & 0xFF, val >> 8);
            break;

        case 'a': // address (V * 256 + offset)
            val = mapProgram.read8(addr++);
            oprLine += fmt::sprintf("VV:%02X", val);
            opcLine += fmt::sprintf(" %02X", val);
            break;

        case 'd': // JRE address
            val = mapProgram.read8(addr++);
            ea = (opCode & 1) ? -(256 - opCode) : +opCode;
            oprLine += fmt::sprintf("$%04X", (addr + ea) & 0xFFFF);
            opcLine += fmt::sprintf(" %02X", val);
            break;

        case 't': // CALT (address)
            val = 0x80 + 2 * (opCode & 0x1F);
            oprLine += fmt::sprintf("($%04X)", val);
            break;

        case 'f': // CALF address
            val = mapProgram.read8(addr++);
            ea = 0x800 + 0x100 * (opCode & 0x07) + val;
            oprLine += fmt::sprintf("$%04X", ea);
            opcLine += fmt::sprintf(" %02X", val);
            break;

        case 'o': // JR offset
            val = ((opCode & 0x20) ? -0x20 : 0) + (opCode & 0x1F);
            oprLine += fmt::sprintf("$%04X", (vAddr + val) & 0xFFFF);
            break;

        case 'i':
            val = mapProgram.read8(addr++);
            oprLine += fmt::sprintf("%s,%d", regName[opCode & 0x1F], opCode >> 5);
            opcLine += fmt::sprintf(" %02X", val);
            break;

        default:
            oprLine += *ch;
        }
    }

    fmt::printf("%-18s %-8s %s\n", opcLine, tokName[opInfo->token], oprLine);

    return addr - vAddr;
}

const upd7810_cpuDevice::dopTable_t upd7810_cpuDevice::dopXX_7810[256] =
{
    // Opcode 00-1F
    { NOP,    nullptr   }, { LDAW,    "%a"      }, { INX,    "SP"      }, { DCX,     "SP"      },
    { LXI,    "SP,%w",  }, { ANIW,    "%a,%b"   }, {                   }, { ANI,     "A,%b"    },
    { MOV,    "A,EAH"   }, { MOV,     "A,EAL"   }, { MOV,    "A,B"     }, { MOV,     "A,C"     },
    { MOV,    "A,D"     }, { MOV,     "A,E"     }, { MOV,    "A,H"     }, { MOV,     "A,L"     },

    { EXA,    nullptr   }, { EXX,     nullptr   }, { INX,    "BC"      }, { DCX,     "BC"      },
    { LXI,    "BC,%w",  }, { ORIW,    "%a,%b"   }, { XRI,    "A,%b"    }, { ORI,     "A,%b"    },
    { MOV,    "EAH,A"   }, { MOV,     "EAL,A"   }, { MOV,    "B,A"     }, { MOV,     "C,A"     },
    { MOV,    "D,A"     }, { MOV,     "E,A"     }, { MOV,    "H,A"     }, { MOV,     "L,A"     },

    // Opcode 20-3F
    { INRW,   "%a"      }, { JB,      nullptr   }, { INX,    "DE"      }, { DCX,     "DE"      },
    { LXI,    "DE,%w",  }, { GTIW,    "%a,%b"   }, { ADINC,  "A,%b"    }, { GTI,     "A,%b"    },
    {                   }, { LDAX,    "(BC)"    }, { LDAX,   "(DE)"    }, { LDAX,    "(HL)"    },
    { LDAX,   "(DE+)"   }, { LDAX,    "(HL+)"   }, { LDAX,   "(DE-)"   }, { LDAX,    "(HL-)"   },

    { DCRW,   "%a"      }, { BLOCK,   nullptr   }, { INX,    "HL"      }, { DCX,     "HL"      },
    { LXI,    "HL,%w",  }, { LTIW,    "%a,%b"   }, { SUINB,  "A,%b"    }, { LTI,     "A,%b"    },
    {                   }, { STAX,    "(BC)"    }, { STAX,   "(DE)"    }, { STAX,    "(HL)"    },
    { STAX,   "(DE+)"   }, { STAX,    "(HL+)"   }, { STAX,   "(DE-)"   }, { STAX,    "(HL-)"   },

    // Opcode 40-5F
    { CALL,   "%w"      }, { INR,     "A"       }, { INR,    "B"       }, { INR,     "C"       },
    { LXI,    "EA,%w",  }, { ONIW,    "%a,%b"   }, { ADI,    "A,%b"    }, { ONI,     "A,%b"    },
    { dop48_7810        }, { MVIX,    "BC,%b"   }, { MVIX,   "DE,%b"   }, { MVIX,    "HL,%b"   },
    { dop4C_7810        }, { dop4D_7810         }, { JRE,    "%d"      }, { JRE,     "%d"      },

    { EXH,    nullptr   }, { DCR,     "A"       }, { DCR,    "B"       }, { DCR,     "C"       },
    { JMP,    "%w",     }, { OFFIW,   "%a,%b"   }, { ACI,    "A,%b"    }, { OFFI,    "A,%b"    },
    { BIT,    "0,%a"    }, { BIT,     "1,%a"    }, { BIT,    "2,%a"    }, { BIT,     "3,%a"    },
    { BIT,    "4,%a"    }, { BIT,     "5,%a"    }, { BIT,    "6,%a"    }, { BIT,     "7,%a"    },

    // Opcode 60-7F
    { dop60_78XX        }, { DAA,     nullptr   }, { RETI,   nullptr   }, { STAW,    "%a"      },
    { dop64_7810        }, { NEIW,    "%a,%b"   }, { SUI,    "A,%b"    }, { NEI,     "A,%b"    },
    { MVI,    "V,%b"    }, { MVI,     "A,%b"    }, { MVI,    "B,%b"    }, { MVI,     "C,%b"    },
    { MVI,    "D,%b"    }, { MVI,     "E,%b"    }, { MVI,    "H,%b"    }, { MVI,     "L,%b"    },

    { dop70_78XX        }, { MVIW,    "%a,%b"   }, { SOFTI,  nullptr   }, {                    },
    { dop74_78XX        }, { EQIW,    "%a,%b"   }, { SBI,    "A,%b"    }, { EQI,     "A,%b"    },
    { CALF,    "%f"     }, { CALF,    "%f"      }, { CALF,   "%f"      }, { CALF,    "%f"      },
    { CALF,    "%f"     }, { CALF,    "%f"      }, { CALF,   "%f"      }, { CALF,    "%f"      },

    // Opcode 80-9F
    { CALT,    "%t"     }, { CALT,    "%t"      }, { CALT,   "%t"      }, { CALT,    "%t"      },
    { CALT,    "%t"     }, { CALT,    "%t"      }, { CALT,   "%t"      }, { CALT,    "%t"      },
    { CALT,    "%t"     }, { CALT,    "%t"      }, { CALT,   "%t"      }, { CALT,    "%t"      },
    { CALT,    "%t"     }, { CALT,    "%t"      }, { CALT,   "%t"      }, { CALT,    "%t"      },

    { CALT,    "%t"     }, { CALT,    "%t"      }, { CALT,   "%t"      }, { CALT,    "%t"      },
    { CALT,    "%t"     }, { CALT,    "%t"      }, { CALT,   "%t"      }, { CALT,    "%t"      },
    { CALT,    "%t"     }, { CALT,    "%t"      }, { CALT,   "%t"      }, { CALT,    "%t"      },
    { CALT,    "%t"     }, { CALT,    "%t"      }, { CALT,   "%t"      }, { CALT,    "%t"      },

    // Opcode A0-BF
    { POP,     "VA"     }, { POP,     "BC"      }, { POP,    "DE"      }, { POP,     "HL"      },
    { POP,     "EA"     }, { DMOV,    "EA,BC"   }, { DMOV,   "EA,DE"   }, { DMOV,    "EA,HL"   },
    { INX,     "EA"     }, { DCX,     "EA"      }, { EI,     nullptr   }, { LDAX,    "(DE+%b)" },
    { LDAX,    "(HL+A)" }, { LDAX,    "(HL+B)"  }, { LDAX,   "(HL+EA)" }, { LDAX,    "(HL+%b)" },

    { PUSH,    "VA"     }, { PUSH,    "BC"      }, { PUSH,   "DE"      }, { PUSH,    "HL"      },
    { PUSH,    "EA"     }, { DMOV,    "BC,EA"   }, { DMOV,   "DE,EA"   }, { DMOV,    "HL,EA"   },
    { RET,     nullptr  }, { RETS,    nullptr   }, { DI,     nullptr   }, { STAX,    "(DE+%b)" },
    { STAX,    "(HL+A)" }, { STAX,    "(HL+B)"  }, { STAX,   "(HL+EA)" }, { STAX,    "(HL+%b)" },

    // Opcode C0-DF
    { JR,      "%o"     }, { JR,      "%o"      }, { JR,     "%o"      }, { JR,      "%o"      },
    { JR,      "%o"     }, { JR,      "%o"      }, { JR,     "%o"      }, { JR,      "%o"      },
    { JR,      "%o"     }, { JR,      "%o"      }, { JR,     "%o"      }, { JR,      "%o"      },
    { JR,      "%o"     }, { JR,      "%o"      }, { JR,     "%o"      }, { JR,      "%o"      },

    { JR,      "%o"     }, { JR,      "%o"      }, { JR,     "%o"      }, { JR,      "%o"      },
    { JR,      "%o"     }, { JR,      "%o"      }, { JR,     "%o"      }, { JR,      "%o"      },
    { JR,      "%o"     }, { JR,      "%o"      }, { JR,     "%o"      }, { JR,      "%o"      },
    { JR,      "%o"     }, { JR,      "%o"      }, { JR,     "%o"      }, { JR,      "%o"      },

    // Opcode E0-FF
    { JR,      "%o"     }, { JR,      "%o"      }, { JR,     "%o"      }, { JR,      "%o"      },
    { JR,      "%o"     }, { JR,      "%o"      }, { JR,     "%o"      }, { JR,      "%o"      },
    { JR,      "%o"     }, { JR,      "%o"      }, { JR,     "%o"      }, { JR,      "%o"      },
    { JR,      "%o"     }, { JR,      "%o"      }, { JR,     "%o"      }, { JR,      "%o"      },

    { JR,      "%o"     }, { JR,      "%o"      }, { JR,     "%o"      }, { JR,      "%o"      },
    { JR,      "%o"     }, { JR,      "%o"      }, { JR,     "%o"      }, { JR,      "%o"      },
    { JR,      "%o"     }, { JR,      "%o"      }, { JR,     "%o"      }, { JR,      "%o"      },
    { JR,      "%o"     }, { JR,      "%o"      }, { JR,     "%o"      }, { JR,      "%o"      }
};

const upd7810_cpuDevice::dopTable_t upd7810_cpuDevice::dop48_7810[256] =
{
    // Opcode 00-1F
    {                   }, { SLRC,    "A"       }, { SLRC,   "B"       }, { SLRC,    "C"       },
    {                   }, { SLLC,    "A"       }, { SLLC,   "B"       }, { SLLC,    "C"       },
    { SK,     "NV"      }, {                    }, { SK,     "CY"      }, { SK,      "HC"      },
    { SK,     "Z"       }, {                    }, {                   }, {                    },

    {                   }, {                    }, {                   }, {                    },
    {                   }, {                    }, {                   }, {                    },
    { SKN,    "NV"      }, {                    }, { SKN,    "CY"      }, { SKN,     "HC"      },
    { SKN,    "Z"       }, {                    }, {                   }, {                    },

    // Opcode 20-3F
    {                   }, { SLR,     "A"       }, { SLR,    "B"       }, { SLR,     "C"       },
    {                   }, { SLL,     "A"       }, { SLL,    "B"       }, { SLL ,    "C"       },
    { JEA,    nullptr   }, { CALB,    nullptr   }, { CLC,    nullptr   }, { STC,     nullptr   },
    {                   }, { MUL,     "A"       }, { MUL,    "B"       }, { MUL,     "C"       },

    {                   }, { RLR,     "A"       }, { RLR,    "B"       }, { RLR,     "C"       },
    {                   }, { RLL,     "A"       }, { RLL,    "B"       }, { RLL ,    "C"       },
    { RLD,    nullptr   }, { RRD,     nullptr   }, { NEGA,   nullptr   }, { HALT,    nullptr   },
    {                   }, { DIV,     "A"       }, { DIV,    "B"       }, { DIV,     "C"       },

    // Opcode 40-5F
    { SKIT,   "NMI"     }, { SKIT,    "FT0"     }, { SKIT,   "FT1"     }, { SKIT,      "F1"    },
    { SKIT,   "F2"      }, { SKIT,    "FE0"     }, { SKIT,   "FE1"     }, { SKIT,      "FEIN"  },
    { SKIT,   "FAD"     }, { SKIT,    "FSR"     }, { SKIT,   "FST"     }, { SKIT,      "ER"    },
    { SKIT,   "OV"      }, {                    }, {                   }, {                    },

    { SKIT,   "AN4"     }, { SKIT,    "AN5"     }, { SKIT,   "AN6"     }, { SKIT,      "AN7"   },
    { SKIT,   "SB"      }, {                    }, {                   }, {                    },
    {                   }, {                    }, {                   }, {                    },
    {                   }, {                    }, {                   }, {                    },

    // Opcode 60-7F
    { SKNIT,  "NMI"     }, { SKNIT,   "FT0"     }, { SKNIT,  "FT1"     }, { SKNIT,     "F1"    },
    { SKNIT,  "F2"      }, { SKNIT,   "FE0"     }, { SKNIT,  "FE1"     }, { SKNIT,     "FEIN"  },
    { SKNIT,  "FAD"     }, { SKNIT,   "FSR"     }, { SKNIT,  "FST"     }, { SKNIT,     "ER"    },
    { SKNIT,  "OV"      }, {                    }, {                   }, {                    },

    { SKNIT,  "AN4"     }, { SKNIT,   "AN5"     }, { SKNIT,  "AN6"     }, { SKNIT,     "AN7"   },
    { SKNIT,  "SB"      }, {                    }, {                   }, {                    },
    {                   }, {                    }, {                   }, {                    },
    {                   }, {                    }, {                   }, {                    },

    // Opcode 80-9F
    {                   }, {                    }, { LDEAX,  "(DE)"    }, { LDEAX,     "(HL)"    },
    { LDEAX,   "(DE++)" }, { LDEAX,   "(HL++)"  }, {                   }, {                      },
    {                   }, {                    }, {                   }, { LDEAX,     "(DE+%b)" },
    { LDEAX,   "(HL+A)" }, { LDEAX,   "(HL+B)"  }, { LDEAX,  "(HL+EA)" }, { LDEAX,     "(HL+%b)" },

    {                   }, {                    }, { STEAX,  "(DE)"    }, { STEAX,     "(HL)"    },
    { STEAX,   "(DE++)" }, { STEAX,   "(HL++)"  }, {                   }, {                      },
    {                   }, {                    }, {                   }, { STEAX,     "(DE+%b)" },
    { STEAX,   "(HL+A)" }, { STEAX,   "(HL+B)"  }, { STEAX,  "(HL+EA)" }, { STEAX,     "(HL+%b)" },

    // Opcode A0-BF
    { DSLR,    "EA"     }, {                    }, {                   }, {                      },
    { DSLL,    "EA"     }, {                    }, {                   }, {                      },
    { TABLE,   nullptr  }, {                    }, {                   }, {                      },
    {                   }, {                    }, {                   }, {                      },

    { DRLR,    "EA"     }, {                    }, {                   }, {                      },
    { DRLL,    "EA"     }, {                    }, {                   }, {                      },
    {                   }, {                    }, {                   }, { STOP, nullptr        },
    {                   }, {                    }, {                   }, {                      },

    // Opcode C0-DF
    { DMOV,    "EA,ECNT" }, { DMOV,   "EA,ECPT"  }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    {                    }, {                    }, { DMOV,   "ETM0,EA" }, { DMOV, "ETM1,EA"      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    // Opcode E0-FF
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      }
};

const upd7810_cpuDevice::dopTable_t upd7810_cpuDevice::dop4C_7810[256] =
{
    // Opcode 00-1F
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    // Opcode 20-3F
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    // Opcode 40-5F
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    // Opcode 60-7F
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    // Opcode 80-9F
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    // Opcode A0-BF
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    // Opcode C0-DF
    { MOV,    "A,PA"     }, { MOV,    "A,PB"     }, { MOV,   "A,PC"     }, { MOV,     "A,PD"      },
    {                    }, { MOV,    "A,PF"     }, { MOV,   "A,MKH"    }, { MOV,     "A,MKL"     },
    { MOV,    "A,ANM"    }, { MOV,    "A,SMH"    }, {                   }, { MOV,     "A,EOM"     },
    {                    }, { MOV,    "A,TMM"    }, {                   }, {                      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, { MOV,    "A,RXB"    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    // Opcode E0-FF
    { MOV,     "A,CR0"   }, { MOV,    "A,CR1"    }, { MOV,   "A,CR2"    }, { MOV,   "A,CR3"       },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      }
};

const upd7810_cpuDevice::dopTable_t upd7810_cpuDevice::dop4D_7810[256] =
{
    // Opcode 00-1F
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    // Opcode 20-3F
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    // Opcode 40-5F
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    // Opcode 60-7F
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    // Opcode 80-9F
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    // Opcode A0-BF
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    // Opcode C0-DF
    { MOV,    "PA,A"     }, { MOV,    "PB,A"     }, { MOV,   "PC,A"     }, { MOV,     "PD,A"      },
    {                    }, { MOV,    "PF,A"     }, { MOV,   "MKH,A"    }, { MOV,     "MKL,A"     },
    { MOV,    "ANM,A"    }, { MOV,    "SMH,A"    }, { MOV,   "SML,A"    }, { MOV,     "EOM,A"     },
    { MOV,    "ETMM,A"   }, { MOV,    "TMM,A"    }, {                   }, {                      },

    { MOV,    "MM,A"     }, { MOV,    "MCC,A"    }, { MOV,   "MA,A"     }, { MOV,     "MB,A"      },
    { MOV,    "MC,A"     }, {                    }, {                   }, { MOV,     "MF,A"      },
    { MOV,    "TXB,A"    }, {                    }, { MOV,   "TM0,A"    }, { MOV,     "TM1,A"     },
    {                    }, {                    }, {                   }, {                      },

    // Opcode E0-FF
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    { MOV,    "ZCM,A"    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      }
};

const upd7810_cpuDevice::dopTable_t upd7810_cpuDevice::dop60_78XX[256] =
{
    // Opcode 00-1F
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    { ANA,   "V,A"       }, { ANA,    "A,A"      }, { ANA,     "B,A"    }, { ANA,    "C,A"        },
    { ANA,   "D,A"       }, { ANA,    "E,A"      }, { ANA,     "H,A"    }, { ANA,    "L,A"        },

    { XRA,   "V,A"       }, { XRA,    "A,A"      }, { XRA,     "B,A"    }, { XRA,    "C,A"        },
    { XRA,   "D,A"       }, { XRA,    "E,A"      }, { XRA,     "H,A"    }, { XRA,    "L,A"        },
    { ORA,   "V,A"       }, { ORA,    "A,A"      }, { ORA,     "B,A"    }, { ORA,    "C,A"        },
    { ORA,   "D,A"       }, { ORA,    "E,A"      }, { ORA,     "H,A"    }, { ORA,    "L,A"        },

    // Opcode 20-3F
    { ADDNC, "V,A"       }, { ADDNC,  "A,A"      }, { ADDNC,   "B,A"    }, { ADDNC,  "C,A"        },
    { ADDNC, "D,A"       }, { ADDNC,  "E,A"      }, { ADDNC,   "H,A"    }, { ADDNC,  "L,A"        },
    { GTA,   "V,A"       }, { GTA,    "A,A"      }, { GTA,     "B,A"    }, { GTA,    "C,A"        },
    { GTA,   "D,A"       }, { GTA,    "E,A"      }, { GTA,     "H,A"    }, { GTA,    "L,A"        },

    { SUBNB, "V,A"       }, { SUBNB,  "A,A"      }, { SUBNB,   "B,A"    }, { SUBNB,  "C,A"        },
    { SUBNB, "D,A"       }, { SUBNB,  "E,A"      }, { SUBNB,   "H,A"    }, { SUBNB,  "L,A"        },
    { LTA,   "V,A"       }, { LTA,    "A,A"      }, { LTA,     "B,A"    }, { LTA,    "C,A"        },
    { LTA,   "D,A"       }, { LTA,    "E,A"      }, { LTA,     "H,A"    }, { LTA,    "L,A"        },

    // Opcode 40-5F
    { ADD,   "V,A"       }, { ADD,    "A,A"      }, { ADD,     "B,A"    }, { ADD,    "C,A"        },
    { ADD,   "D,A"       }, { ADD,    "E,A"      }, { ADD,     "H,A"    }, { ADD,    "L,A"        },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    { ADC,   "V,A"       }, { ADC,    "A,A"      }, { ADC,     "B,A"    }, { ADC,    "C,A"        },
    { ADC,   "D,A"       }, { ADC,    "E,A"      }, { ADC,     "H,A"    }, { ADC,    "L,A"        },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    // Opcode 60-7F
    { SUB,   "V,A"       }, { SUB,    "A,A"      }, { SUB,     "B,A"    }, { SUB,    "C,A"        },
    { SUB,   "D,A"       }, { SUB,    "E,A"      }, { SUB,     "H,A"    }, { SUB,    "L,A"        },
    { NEA,   "V,A"       }, { NEA,    "A,A"      }, { NEA,     "B,A"    }, { NEA,    "C,A"        },
    { NEA,   "D,A"       }, { NEA,    "E,A"      }, { NEA,     "H,A"    }, { NEA,    "L,A"        },

    { SBB,   "V,A"       }, { SBB,    "A,A"      }, { SBB,     "B,A"    }, { SBB,    "C,A"        },
    { SBB,   "D,A"       }, { SBB,    "E,A"      }, { SBB,     "H,A"    }, { SBB,    "L,A"        },
    { EQA,   "V,A"       }, { EQA,    "A,A"      }, { EQA,     "B,A"    }, { EQA,    "C,A"        },
    { EQA,   "D,A"       }, { EQA,    "E,A"      }, { EQA,     "H,A"    }, { EQA,    "L,A"        },

    // Opcode 80-9F
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    { ANA,   "A,V"       }, { ANA,    "A,A"      }, { ANA,     "A,B"    }, { ANA,    "A,C"        },
    { ANA,   "A,D"       }, { ANA,    "A,E"      }, { ANA,     "A,H"    }, { ANA,    "A,L"        },

    { XRA,   "A,V"       }, { XRA,    "A,A"      }, { XRA,     "A,B"    }, { XRA,    "A,C"        },
    { XRA,   "A,D"       }, { XRA,    "A,E"      }, { XRA,     "A,H"    }, { XRA,    "A,L"        },
    { ORA,   "A,V"       }, { ORA,    "A,A"      }, { ORA,     "A,B"    }, { ORA,    "A,C"        },
    { ORA,   "A,D"       }, { ORA,    "A,E"      }, { ORA,     "A,H"    }, { ORA,    "A,L"        },

    // Opcode A0-BF
    { ADDNC, "A,V"       }, { ADDNC,  "A,A"      }, { ADDNC,   "A,B"    }, { ADDNC,  "A,C"        },
    { ADDNC, "A,D"       }, { ADDNC,  "A,E"      }, { ADDNC,   "A,H"    }, { ADDNC,  "A,L"        },
    { GTA,   "A,V"       }, { GTA,    "A,A"      }, { GTA,     "A,B"    }, { GTA,    "A,C"        },
    { GTA,   "A,D"       }, { GTA,    "A,E"      }, { GTA,     "A,H"    }, { GTA,    "A,L"        },

    { SUBNB, "A,V"       }, { SUBNB,  "A,A"      }, { SUBNB,   "A,B"    }, { SUBNB,  "A,C"        },
    { SUBNB, "A,D"       }, { SUBNB,  "A,E"      }, { SUBNB,   "A,H"    }, { SUBNB,  "A,L"        },
    { LTA,   "A,V"       }, { LTA,    "A,A"      }, { LTA,     "A,B"    }, { LTA,    "A,C"        },
    { LTA,   "A,V"       }, { LTA,    "A,E"      }, { LTA,     "A,H"    }, { LTA,    "A,L"        },

    // Opcode C0-DF
    { ADD,   "A,V"       }, { ADD,    "A,A"      }, { ADD,     "B,A"    }, { ADD,    "A,C"        },
    { ADD,   "A,D"       }, { ADD,    "E,A"      }, { ADD,     "H,A"    }, { ADD,    "A,L"        },
    { ONA,   "A,V"       }, { ONA,    "A,A"      }, { ONA,     "A,B"    }, { ONA,    "A,C"        },
    { ONA,   "A,D"       }, { ONA,    "A,E"      }, { ONA,     "A,H"    }, { ONA,    "A,L"        },

    { ADC,   "A,V"       }, { ADC,    "A,A"      }, { ADC,     "A,B"    }, { ADC,    "A,C"        },
    { ADC,   "A,D"       }, { ADC,    "A,E"      }, { ADC,     "A,H"    }, { ADC,    "A,L"        },
    { OFFA,  "A,V"       }, { OFFA,   "A,A"      }, { OFFA,    "A,B"    }, { OFFA,   "A,C"        },
    { OFFA,  "A,D"       }, { OFFA,   "A,E"      }, { OFFA,    "A,H"    }, { OFFA,   "A,L"        },

    // Opcode E0-FF
    { SUB,   "A,V"       }, { SUB,    "A,A"      }, { SUB,     "A,B"    }, { SUB,    "A,C"        },
    { SUB,   "A,D"       }, { SUB,    "A,E"      }, { SUB,     "A,H"    }, { SUB,    "A,L"        },
    { NEA,   "A,V"       }, { NEA,    "A,A"      }, { NEA,     "A,B"    }, { NEA,    "A,C"        },
    { NEA,   "A,D"       }, { NEA,    "A,E"      }, { NEA,     "A,H"    }, { NEA,    "A,L"        },

    { SBB,   "A,V"       }, { SBB,    "A,A"      }, { SBB,     "A,B"    }, { SBB,    "A,C"        },
    { SBB,   "A,D"       }, { SBB,    "A,E"      }, { SBB,     "A,H"    }, { SBB,    "A,L"        },
    { EQA,   "A,V"       }, { EQA,    "A,A"      }, { EQA,     "A,B"    }, { EQA,    "A,C"        },
    { EQA,   "A,D"       }, { EQA,    "A,E"      }, { EQA,     "A,H"    }, { EQA,    "A,L"        }
};

const upd7810_cpuDevice::dopTable_t upd7810_cpuDevice::dop64_7810[256] =
{
    // Opcode 00-1F
    { MVI,    "PA,%b"    }, { MVI,    "PB,%b"    }, { MVI,     "PC,%b"  }, { MVI,    "PD,%b"      },
    {                    }, { MVI,    "PF,%b"    }, { MVI,     "MKH,%b" }, { MVI,    "MKL,%b"     },
    { ANI,    "PA,%b"    }, { ANI,    "PB,%b"    }, { ANI,     "PC,%b"  }, { ANI,    "PD,%b"      },
    {                    }, { ANI,    "PF,%b"    }, { ANI,     "MKH,%b" }, { ANI,    "MKL,%b"     },

    { XRI,    "PA,%b"    }, { XRI,    "PB,%b"    }, { XRI,     "PC,%b"  }, { XRI,    "PD,%b"      },
    {                    }, { XRI,    "PF,%b"    }, { XRI,     "MKH,%b" }, { XRI,    "MKL,%b"     },
    { ORI,    "PA,%b"    }, { ORI,    "PB,%b"    }, { ORI,     "PC,%b"  }, { ORI,    "PD,%b"      },
    {                    }, { ORI,    "PF,%b"    }, { ORI,     "MKH,%b" }, { ORI,    "MKL,%b"     },

    // Opcode 20-3F
    { ADINC,  "PA,%b"    }, { ADINC,  "PB,%b"    }, { ADINC,   "PC,%b"  }, { ADINC,  "PD,%b"      },
    {                    }, { ADINC,  "PF,%b"    }, { ADINC,   "MKH,%b" }, { ADINC,  "MKL,%b"     },
    { GTI,    "PA,%b"    }, { GTI,    "PB,%b"    }, { GTI,     "PC,%b"  }, { GTI,    "PD,%b"      },
    {                    }, { GTI,    "PF,%b"    }, { GTI,     "MKH,%b" }, { GTI,    "MKL,%b"     },

    { SUINB,  "PA,%b"    }, { SUINB,  "PB,%b"    }, { SUINB,   "PC,%b"  }, { SUINB,  "PD,%b"      },
    {                    }, { SUINB,  "PF,%b"    }, { SUINB,   "MKH,%b" }, { SUINB,  "MKL,%b"     },
    { LTI,    "PA,%b"    }, { LTI,    "PB,%b"    }, { LTI,     "PC,%b"  }, { LTI,    "PD,%b"      },
    {                    }, { LTI,    "PF,%b"    }, { LTI,     "MKH,%b" }, { LTI,    "MKL,%b"     },

    // Opcode 40-5F
    { ADI,    "PA,%b"    }, { ADI,    "PB,%b"    }, { ADI,     "PC,%b"  }, { ADI,    "PD,%b"      },
    {                    }, { ADI,    "PF,%b"    }, { ADI,     "MKH,%b" }, { ADI,    "MKL,%b"     },
    { ONI,    "PA,%b"    }, { ONI,    "PB,%b"    }, { ONI,     "PC,%b"  }, { ONI,    "PD,%b"      },
    {                    }, { ONI,    "PF,%b"    }, { ONI,     "MKH,%b" }, { ONI,    "MKL,%b"     },

    { ACI,    "PA,%b"    }, { ACI,    "PB,%b"    }, { ACI,     "PC,%b"  }, { ACI,    "PD,%b"      },
    {                    }, { ACI,    "PF,%b"    }, { ACI,     "MKH,%b" }, { ACI,    "MKL,%b"     },
    { OFFI,   "PA,%b"    }, { OFFI,   "PB,%b"    }, { OFFI,    "PC,%b"  }, { OFFI,   "PD,%b"      },
    {                    }, { OFFI,   "PF,%b"    }, { OFFI,    "MKH,%b" }, { OFFI,   "MKL,%b"     },

    // Opcode 60-7F
    { SUI,    "PA,%b"    }, { SUI,    "PB,%b"    }, { SUI,     "PC,%b"  }, { SUI,    "PD,%b"      },
    {                    }, { SUI,    "PF,%b"    }, { SUI,     "MKH,%b" }, { SUI,    "MKL,%b"     },
    { NEI,    "PA,%b"    }, { NEI,    "PB,%b"    }, { NEI,     "PC,%b"  }, { NEI,    "PD,%b"      },
    {                    }, { NEI,    "PF,%b"    }, { NEI,     "MKH,%b" }, { NEI,    "MKL,%b"     },

    { SBI,    "PA,%b"    }, { SBI,    "PB,%b"    }, { SBI,     "PC,%b"  }, { SBI,    "PD,%b"      },
    {                    }, { SBI,    "PF,%b"    }, { SBI,     "MKH,%b" }, { SBI,    "MKL,%b"     },
    { EQI,    "PA,%b"    }, { EQI,    "PB,%b"    }, { EQI,     "PC,%b"  }, { EQI,    "PD,%b"      },
    {                    }, { EQI,    "PF,%b"    }, { EQI,     "MKH,%b" }, { EQI,    "MKL,%b"     },

    // Opcode 80-9F
    { MVI,    "ANM,%b"   }, { MVI,    "SMH,%b"   }, {                   }, { MVI,    "EOM,%b"     },
    {                    }, { MVI,    "TMM,%b"   }, {                   }, {                      },
    { ANI,    "ANM,%b"   }, { ANI,    "SMH,%b"   }, {                   }, { ANI,    "EOM,%b"     },
    {                    }, { ANI,    "TMM,%b"   }, {                   }, {                      },

    { XRI,    "ANM,%b"   }, { XRI,    "SMH,%b"   }, {                   }, { XRI,    "EOM,%b"     },
    {                    }, { XRI,    "TMM,%b"   }, {                   }, {                      },
    { ORI,    "ANM,%b"   }, { ORI,    "SMH,%b"   }, {                   }, { ORI,    "EOM,%b"     },
    {                    }, { ORI,    "TMM,%b"   }, {                   }, {                      },

    // Opcode A0-BF
    { ADINC,  "ANM,%b"   }, { ADINC,  "SMH,%b"   }, {                   }, { ADINC,  "EOM,%b"     },
    {                    }, { ADINC,  "TMM,%b"   }, {                   }, {                      },
    { GTI,    "ANM,%b"   }, { GTI,    "SMH,%b"   }, {                   }, { GTI,    "EOM,%b"     },
    {                    }, { GTI,    "TMM,%b"   }, {                   }, {                      },

    { SUINB,  "ANM,%b"   }, { SUINB,  "SMH,%b"   }, {                   }, { SUINB,  "EOM,%b"     },
    {                    }, { SUINB,  "TMM,%b"   }, {                   }, {                      },
    { LTI,    "ANM,%b"   }, { LTI,    "SMH,%b"   }, {                   }, { LTI,    "EOM,%b"     },
    {                    }, { LTI,    "TMM,%b"   }, {                   }, {                      },

    // Opcode C0-DF
    { ADI,    "ANM,%b"   }, { ADI,    "SMH,%b"   }, {                   }, { ADI,    "EOM,%b"     },
    {                    }, { ADI,    "TMM,%b"   }, {                   }, {                      },
    { ONI,    "ANM,%b"   }, { ONI,    "SMH,%b"   }, {                   }, { ONI,    "EOM,%b"     },
    {                    }, { ONI,    "TMM,%b"   }, {                   }, {                      },

    { ACI,    "ANM,%b"   }, { ACI,    "SMH,%b"   }, {                   }, { ACI,    "EOM,%b"     },
    {                    }, { ACI,    "TMM,%b"   }, {                   }, {                      },
    { OFFI,   "ANM,%b"   }, { OFFI,   "SMH,%b"   }, {                   }, { OFFI,   "EOM,%b"     },
    {                    }, { OFFI,   "TMM,%b"   }, {                   }, {                      },

    // Opcode E0-FF
    { SUI,    "ANM,%b"   }, { SUI,    "SMH,%b"   }, {                   }, { SUI,    "EOM,%b"     },
    {                    }, { SUI,    "TMM,%b"   }, {                   }, {                      },
    { NEI,    "ANM,%b"   }, { NEI,    "SMH,%b"   }, {                   }, { NEI,    "EOM,%b"     },
    {                    }, { NEI,    "TMM,%b"   }, {                   }, {                      },

    { SBI,    "ANM,%b"   }, { SBI,    "SMH,%b"   }, {                   }, { SBI,    "EOM,%b"     },
    {                    }, { SBI,    "TMM,%b"   }, {                   }, {                      },
    { EQI,    "ANM,%b"   }, { EQI,    "SMH,%b"   }, {                   }, { EQI,    "EOM,%b"     },
    {                    }, { EQI,    "TMM,%b"   }, {                   }, {                      }
};

const upd7810_cpuDevice::dopTable_t upd7810_cpuDevice::dop70_78XX[256] =
{
    // Opcode 00-1F
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, { SSPD,     "%w"    }, { LSPD,      "%w"      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, { SBCD,     "%w"    }, { LBCD,      "%w"      },
  
    // Opcode 20-3F
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, { SDED,     "%w"    }, { LDED,      "%w"      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, { SHLD,     "%w"    }, { LHLD,      "%w"      },

    // Opcode 40-5F
    { EADD,    "EA,V"    }, { EADD,    "EA,A"    }, { EADD,     "EA,B"  }, { EADD,      "EA,C"    },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
   
    // Opcode 60-7F
    { ESUB,    "EA,V"    }, { ESUB,    "EA,A"    }, { ESUB,     "EA,B"  }, { ESUB,      "EA,C"    },
    {                    }, {                    }, {                   }, {                      },
    { MOV,     "V,(%w)"  }, { MOV,     "A,(%w)"  }, { MOV,     "B,(%w)" }, { MOV,       "C,(%w)"  },
    { MOV,     "D,(%w)"  }, { MOV,     "E,(%w)"  }, { MOV,     "H,(%w)" }, { MOV,       "L,(%w)"  },

    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    { MOV,     "(%w),V"  }, { MOV,     "(%w),A"  }, { MOV,     "(%w),B" }, { MOV,       "(%w),C"  },
    { MOV,     "(%w),D"  }, { MOV,     "(%w),E"  }, { MOV,     "(%w),H" }, { MOV,       "(%w),L"  },

    // Opcode 80-9F
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    {                    }, { ANAX,    "(BC)"    }, { ANAX,    "(DE)"   }, { ANAX,      "(HL)"    },
    { ANAX,    "(DE+)"   }, { ANAX,    "(HL+)"   }, { ANAX,    "(DE-)"  }, { ANAX,      "(HL-)"   },

    {                    }, { XRAX,    "(BC)"    }, { XRAX,    "(DE)"   }, { XRAX,      "(HL)"    },
    { XRAX,    "(DE+)"   }, { XRAX,    "(HL+)"   }, { XRAX,    "(DE-)"  }, { XRAX,      "(HL-)"   },
    {                    }, { ORAX,    "(BC)"    }, { ORAX,    "(DE)"   }, { ORAX,      "(HL)"    },
    { ORAX,    "(DE+)"   }, { ORAX,    "(HL+)"   }, { ORAX,    "(DE-)"  }, { ORAX,      "(HL-)"   },

    // Opcode A0-BF
    {                    }, { ADDNCX,  "(BC)"    }, { ADDNCX,  "(DE)"   }, { ADDNCX,    "(HL)"    },
    { ADDNCX,  "(DE+)"   }, { ADDNCX,  "(HL+)"   }, { ADDNCX,  "(DE-)"  }, { ADDNCX,    "(HL-)"   },
    {                    }, { GTAX,    "(BC)"    }, { GTAX,    "(DE)"   }, { GTAX,      "(HL)"    },
    { GTAX,    "(DE+)"   }, { GTAX,    "(HL+)"   }, { GTAX,    "(DE-)"  }, { GTAX,      "(HL-)"   },

    {                    }, { SUBNBX,  "(BC)"    }, { SUBNBX,  "(DE)"   }, { SUBNBX,    "(HL)"    },
    { SUBNBX,  "(DE+)"   }, { SUBNBX,  "(HL+)"   }, { SUBNBX,  "(DE-)"  }, { SUBNBX,    "(HL-)"   },
    {                    }, { LTAX,    "(BC)"    }, { LTAX,    "(DE)"   }, { LTAX,      "(HL)"    },
    { LTAX,    "(DE+)"   }, { LTAX,    "(HL+)"   }, { LTAX,    "(DE-)"  }, { LTAX,      "(HL-)"   },

    // Opcode C0-DF
    {                    }, { ADDX,    "(BC)"    }, { ADDX,    "(DE)"   }, { ADDX,      "(HL)"    },
    { ADDX,    "(DE+)"   }, { ADDX,    "(HL+)"   }, { ADDX,    "(DE-)"  }, { ADDX,      "(HL-)"   },
    {                    }, { ONAX,    "(BC)"    }, { ONAX,    "(DE)"   }, { ONAX,      "(HL)"    },
    { ONAX,    "(DE+)"   }, { ONAX,    "(HL+)"   }, { ONAX,    "(DE-)"  }, { ONAX,      "(HL-)"   },

    {                    }, { ADCX,    "(BC)"    }, { ADCX,    "(DE)"   }, { ADCX,      "(HL)"    },
    { ADCX,    "(DE+)"   }, { ADCX,    "(HL+)"   }, { ADCX,    "(DE-)"  }, { ADCX,      "(HL-)"   },
    {                    }, { OFFAX,   "(BC)"    }, { OFFAX,   "(DE)"   }, { OFFAX,     "(HL)"    },
    { OFFAX,   "(DE+)"   }, { OFFAX,   "(HL+)"   }, { OFFAX,   "(DE-)"  }, { OFFAX,     "(HL-)"   },

    // Opcode E0-FF
    {                    }, { SUBX,    "(BC)"    }, { SUBX,    "(DE)"   }, { SUBX,      "(HL)"    },
    { SUBX,    "(DE+)"   }, { SUBX,    "(HL+)"   }, { SUBX,    "(DE-)"  }, { SUBX,      "(HL-)"   },
    {                    }, { NEAX,    "(BC)"    }, { NEAX,    "(DE)"   }, { NEAX,      "(HL)"    },
    { NEAX,    "(DE+)"   }, { NEAX,    "(HL+)"   }, { NEAX,    "(DE-)"  }, { NEAX,      "(HL-)"   },

    {                    }, { SBBX,    "(BC)"    }, { SBBX,    "(DE)"   }, { SBBX,      "(HL)"    },
    { SBBX,    "(DE+)"   }, { SBBX,    "(HL+)"   }, { SBBX,    "(DE-)"  }, { SBBX,      "(HL-)"   },
    {                    }, { EQAX,    "(BC)"    }, { EQAX,    "(DE)"   }, { EQAX,      "(HL)"    },
    { EQAX,    "(DE+)"   }, { EQAX,    "(HL+)"   }, { EQAX,    "(DE-)"  }, { EQAX,      "(HL-)"   }
};

const upd7810_cpuDevice::dopTable_t upd7810_cpuDevice::dop74_78XX[256] =
{
    // Opcode 00-1F
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    { ANI,   "V,%b"      }, { ANI,    "A,%b"     }, { ANI,     "B,%b"   }, { ANI,    "C,%b"       },
    { ANI,   "D,%b"      }, { ANI,    "E,%b"     }, { ANI,     "H,%b"   }, { ANI,    "L,%b"       },

    { XRI,   "V,%b"      }, { XRI,    "A,%b"     }, { XRI,     "B,%b"   }, { XRI,    "C,%b"       },
    { XRI,   "D,%b"      }, { XRI,    "E,%b"     }, { XRI,     "H,%b"   }, { XRI,    "L,%b"       },
    { ORI,   "V,%b"      }, { ORI,    "A,%b"     }, { ORI,     "B,%b"   }, { ORI,    "C,%b"       },
    { ORI,   "D,%b"      }, { ORI,    "E,%b"     }, { ORI,     "H,%b"   }, { ORI,    "L,%b"       },

    // Opcode 20-3F
    { ADINC, "V,%b"      }, { ADINC,  "A,%b"     }, { ADINC,   "B,%b"   }, { ADINC,  "C,%b"       },
    { ADINC, "D,%b"      }, { ADINC,  "E,%b"     }, { ADINC,   "H,%b"   }, { ADINC,  "L,%b"       },
    { GTI,   "V,%b"      }, { GTI,    "A,%b"     }, { GTI,     "B,%b"   }, { GTI,    "C,%b"       },
    { GTI,   "D,%b"      }, { GTI,    "E,%b"     }, { GTI,     "H,%b"   }, { GTI,    "L,%b"       },

    { SUINB, "V,%b"      }, { SUINB,  "A,%b"     }, { SUINB,   "B,%b"   }, { SUINB,  "C,%b"       },
    { SUINB, "D,%b"      }, { SUINB,  "E,%b"     }, { SUINB,   "H,%b"   }, { SUINB,  "L,%b"       },
    { LTI,   "V,%b"      }, { LTI,    "A,%b"     }, { LTI,     "B,%b"   }, { LTI,    "C,%b"       },
    { LTI,   "D,%b"      }, { LTI,    "E,%b"     }, { LTI,     "H,%b"   }, { LTI,    "L,%b"       },

    // Opcode 40-5F
    { ADI,   "V,%b"      }, { ADI,    "A,%b"     }, { ADI,     "B,%b"   }, { ADI,    "C,%b"       },
    { ADI,   "D,%b"      }, { ADI,    "E,%b"     }, { ADI,     "H,%b"   }, { ADI,    "L,%b"       },
    { ONI,   "V,%b"      }, { ONI,    "A,%b"     }, { ONI,     "B,%b"   }, { ONI,    "C,%b"       },
    { ONI,   "D,%b"      }, { ONI,    "E,%b"     }, { ONI,     "H,%b"   }, { ONI,    "L,%b"       },

    { ACI,   "V,%b"      }, { ACI,    "A,%b"     }, { ACI,     "B,%b"   }, { ACI,    "C,%b"       },
    { ACI,   "D,%b"      }, { ACI,    "E,%b"     }, { ACI,     "H,%b"   }, { ACI,    "L,%b"       },
    { OFFI,  "V,%b"      }, { OFFI,   "A,%b"     }, { OFFI,    "B,%b"   }, { OFFI,   "C,%b"       },
    { OFFI,  "D,%b"      }, { OFFI,   "E,%b"     }, { OFFI,    "H,%b"   }, { OFFI,   "L,%b"       },

    // Opcode 60-7F
    { SUI,   "V,%b"      }, { SUI,    "A,%b"     }, { SUI,     "B,%b"   }, { SUI,    "C,%b"       },
    { SUI,   "D,%b"      }, { SUI,    "E,%b"     }, { SUI,     "H,%b"   }, { SUI,    "L,%b"       },
    { NEI,   "V,%b"      }, { NEI,    "A,%b"     }, { NEI,     "B,%b"   }, { NEI,    "C,%b"       },
    { NEI,   "D,%b"      }, { NEI,    "E,%b"     }, { NEI,     "H,%b"   }, { NEI,    "L,%b"       },

    { SBI,   "V,%b"      }, { SBI,    "A,%b"     }, { SBI,     "B,%b"   }, { SBI,    "C,%b"       },
    { SBI,   "D,%b"      }, { SBI,    "E,%b"     }, { SBI,     "H,%b"   }, { SBI,    "L,%b"       },
    { EQI,   "V,%b"      }, { EQI,    "A,%b"     }, { EQI,     "B,%b"   }, { EQI,    "C,%b"       },
    { EQI,   "D,%b"      }, { EQI,    "E,%b"     }, { EQI,     "H,%b"   }, { EQI,    "L,%b"       },

    // Opcode 80-9F
    {                    }, {                    }, {                   }, {                      },
    {                    }, {                    }, {                   }, {                      },
    { ANAW,   "%a"       }, {                    }, {                   }, {                      },
    {                    }, { DAN,    "EA,BC"    }, { DAN,     "EA,DE"  }, { DAN,    "EA,HL"      },

    { XRAW,   "%a"       }, {                    }, {                   }, {                      },
    {                    }, { DXR,    "EA,BC"    }, { DXR,     "EA,DE"  }, { DXR,    "EA,HL"      },
    { ORAW,   "%a"       }, {                    }, {                   }, {                      },
    {                    }, { DOR,    "EA,BC"    }, { DOR,     "EA,DE"  }, { DOR,    "EA,HL"      },

    // Opcode A0-BF
    { ADDNCW, "%a"       }, {                    }, {                   }, {                      },
    {                    }, { DADDNC, "EA,BC"    }, { DADDNC,  "EA,DE"  }, { DADDNC, "EA,HL"      },
    { GTAW,   "%a"       }, {                    }, {                   }, {                      },
    {                    }, { DGT,    "EA,BC"    }, { DGT,     "EA,DE"  }, { DGT,    "EA,HL"      },

    { SUBNBW, "%a"       }, {                    }, {                   }, {                      },
    {                    }, { DSUBNB, "EA,BC"    }, { DSUBNB,  "EA,DE"  }, { DSUBNB, "EA,HL"      },
    { LTAW,   "%a"       }, {                    }, {                   }, {                      },
    {                    }, { DLT,    "EA,BC"    }, { DLT,     "EA,DE"  }, { DLT,    "EA,HL"      },

    // Opcode C0-BF
    { ADDW,   "%a"       }, {                    }, {                   }, {                      },
    {                    }, { DADD,   "EA,BC"    }, { DADD,    "EA,DE"  }, { DADD,   "EA,HL"      },
    { ONAW,   "%a"       }, {                    }, {                   }, {                      },
    {                    }, { DON,    "EA,BC"    }, { DON,     "EA,DE"  }, { DON,    "EA,HL"      },

    { ADCW,   "%a"       }, {                    }, {                   }, {                      },
    {                    }, { DADC,   "EA,BC"    }, { DADC,    "EA,DE"  }, { DADC,   "EA,HL"      },
    { OFFAW,   "%a"      }, {                    }, {                   }, {                      },
    {                    }, { DOFF,    "EA,BC"   }, { DOFF,    "EA,DE"  }, { DOFF,   "EA,HL"      },

    // Opcode E0-FF
    { SUBW,   "%a"       }, {                    }, {                   }, {                      },
    {                    }, { DSUB,   "EA,BC"    }, { DSUB,    "EA,DE"  }, { DSUB,   "EA,HL"      },
    { NEAW,   "%a"       }, {                    }, {                   }, {                      },
    {                    }, { DNE,    "EA,BC"    }, { DNE,     "EA,DE"  }, { DNE,    "EA,HL"      },

    { SBBW,   "%a"       }, {                    }, {                   }, {                      },
    {                    }, { DSBB,   "EA,BC"    }, { DSBB,    "EA,DE"  }, { DSBB,   "EA,HL"      },
    { EQAW,   "%a"       }, {                    }, {                   }, {                      },
    {                    }, { DEQ,    "EA,BC"    }, { DEQ,     "EA,DE"  }, { DEQ,    "EA,HL"      }
};
