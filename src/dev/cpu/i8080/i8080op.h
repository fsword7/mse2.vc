// i8080.h - Intel 8080/8085 Processor package - opcodes
//
// Author:  Tim Stark
// Date:    12/27/2021

#define DO_NOP      // Do nothing

#define DO_LDAX     REG_A = read8(REGW(opCode))
#define DO_LDA      REG_A = read8(tval)
#define DO_LHLD     REG_HL = read8(tval) | (read8(tval+1) << 8)
#define DO_LXI      REGW(opCode) = tval

#define DO_STAX     write8(REGW(opCode), REG_A)
#define DO_STA      write8(tval, REG_A)
#define DO_SHLD     write8(tval, REG_HL); write8(tval+1, REG_HL >> 8)

#define DO_MOV      REGB(opCode) = REGA(opCode)
#define DO_MOVMr    write8(REG_HL, REGA(opCode))
#define DO_MOVrM    REGB(opCode) = read8(REG_HL)
#define DO_MVI      REGB(opCode) = tval
#define DO_MVIM     write8(REG_HL, tval)

#define DO_INR      REGB(opCode) = opINR(REGB(opCode))
#define DO_DCR      REGB(opCode) = opDCR(REGB(opCode))
#define DO_INRM     write8(REG_HL, opINR(read8(REG_HL)))
#define DO_DCRM     write8(REG_HL, opDCR(read8(REG_HL)))
#define DO_INX      REGW(opCode)++
#define DO_DCX      REGW(opCode)--

#define DO_RLC                                       \
        REG_A = (REG_A << 1) | (REG_A >> 7);         \
        REG_F = (REG_F & ~PSW_CF) | (REG_A & PSW_CF)

#define DO_RRC                                        \
        REG_F = (REG_F & ~PSW_CF) | (REG_A & PSW_CF); \
        REG_A = (REG_A >> 1) | (REG_A << 7)

#define DO_RAL                                               \
        cFlag = REG_A & PSW_CF;                              \
        REG_F = (REG_F & ~PSW_CF) | ((REG_A >> 7) & PSW_CF); \
        REG_A = (REG_A << 1) | cFlag

#define DO_RAR                                               \
        cFlag = (REG_A & PSW_CF) << 7;                       \
        REG_F = (REG_F & ~PSW_CF) | ((REG_A >> 7) & PSW_CF); \
        REG_A = (REG_A >> 1) | cFlag

#define DO_DAD      opDAD(REGW(opCode))

#define DO_ADD      opADD(REGA(opCode))
#define DO_ADC      opADC(REGA(opCode))
#define DO_SUB      opSUB(REGA(opCode))
#define DO_SBB      opSBB(REGA(opCode))
#define DO_ANA      opANA(REGA(opCode))
#define DO_XRA      opXRA(REGA(opCode))
#define DO_ORA      opORA(REGA(opCode))
#define DO_CMP      opCMP(REGA(opCode))

#define DO_ADDM     opADD(read8(REG_HL))
#define DO_ADCM     opADC(read8(REG_HL))
#define DO_SUBM     opSUB(read8(REG_HL))
#define DO_SBBM     opSBB(read8(REG_HL))
#define DO_ANAM     opANA(read8(REG_HL))
#define DO_XRAM     opXRA(read8(REG_HL))
#define DO_ORAM     opORA(read8(REG_HL))
#define DO_CMPM     opCMP(read8(REG_HL))

#define DO_ADI      opADD(tval)
#define DO_ACI      opADC(tval)
#define DO_SUI      opSUB(tval)
#define DO_SBI      opSBB(tval)
#define DO_ANI      opANA(tval)
#define DO_XRI      opXRA(tval)
#define DO_ORI      opORA(tval)
#define DO_CPI      opCMP(tval)

#define DO_STC      REG_F |= PSW_CF
#define DO_CMC      REG_F ^= PSW_CF
#define DO_CMA      REG_A ^= 0xFF

#define DO_RST      opRST(REGBn(opCode))
#define DO_HLT
#define DO_EI       enableInterrupts(true)
#define DO_DI       enableInterrupts(false)
