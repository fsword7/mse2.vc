// mcs48op.cpp - MCS 48 Processor series package - Opcode table
//
// Author:  Tim Stark
// Date:    1/3/22

#include "emu/core.h"
#include "emu/devcb.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "dev/cpu/mcs48/mcs48.h"

#define OP(name) &mcs48_cpuDevice::op##name

const mcs48_cpuDevice::opExecute mcs48_cpuDevice::mcs48_Opcodes[256] =
{
    OP(NOP),        OP(Illegal),    OP(OUTL_BUS_A), OP(ADD_A_N),    OP(JMP_0),      OP(EN_I),       OP(Illegal),   OP(DEC_A),       // 00-07
    OP(INS_A_BUS),  OP(IN_A_P1),    OP(IN_A_P2),    OP(Illegal),    OP(MOVD_A_P4),  OP(MOVD_A_P5),  OP(MOVD_A_P6), OP(MOVD_A_P7),   // 08-0F
    OP(INC_XR0),    OP(INC_XR1),    OP(JB_0),       OP(ADC_A_N),    OP(CALL_0),     OP(DIS_I),      OP(JTF),       OP(INC_A),       // 10-17
    OP(INC_R0),     OP(INC_R1),     OP(INC_R2),     OP(INC_R3),     OP(INC_R4),     OP(INC_R5),     OP(INC_R6),    OP(INC_R7),      // 18-1F
    OP(XCH_A_XR0),  OP(XCH_A_XR1),  OP(Illegal),    OP(MOV_A_N),    OP(JMP_1),      OP(EN_TCNTI),   OP(JNT_0),     OP(CLR_A),       // 20-27
    OP(XCH_A_R0),   OP(XCH_A_R1),   OP(XCH_A_R2),   OP(XCH_A_R3),   OP(XCH_A_R4),   OP(XCH_A_R5),   OP(XCH_A_R6),  OP(XCH_A_R7),    // 28-2F
    OP(XCHD_A_XR0), OP(XCHD_A_XR1), OP(JB_1),       OP(Illegal),    OP(CALL_1),     OP(DIS_TCNTI),  OP(JT_0),      OP(CPL_A),       // 30-37
    OP(Illegal),    OP(OUTL_P1_A),  OP(OUTL_P2_A),  OP(Illegal),    OP(MOVD_P4_A),  OP(MOVD_P5_A),  OP(MOVD_P6_A), OP(MOVD_P7_A),   // 38-3F
    OP(ORL_A_XR0),  OP(ORL_A_XR1),  OP(MOV_A_T),    OP(ORL_A_N),    OP(JMP_2),      OP(STRT_CNT),   OP(JNT_1),     OP(SWAP_A),      // 40-47
    OP(ORL_A_R0),   OP(ORL_A_R1),   OP(ORL_A_R2),   OP(ORL_A_R3),   OP(ORL_A_R4),   OP(ORL_A_R5),   OP(ORL_A_R6),  OP(ORL_A_R7),    // 48-4F
    OP(ANL_A_XR0),  OP(ANL_A_XR1),  OP(JB_2),       OP(ANL_A_N),    OP(CALL_2),     OP(STRT_T),     OP(JT_1),      OP(DA_A),        // 50-57
    OP(ANL_A_R0),   OP(ANL_A_R1),   OP(ANL_A_R2),   OP(ANL_A_R3),   OP(ANL_A_R4),   OP(ANL_A_R5),   OP(ANL_A_R6),  OP(ANL_A_R7),    // 58-5F
    OP(ADD_A_XR0),  OP(ADD_A_XR1),  OP(MOV_T_A),    OP(Illegal),    OP(JMP_3),      OP(STOP_TCNT),  OP(Illegal),   OP(RRC_A),       // 60-67
    OP(ADD_A_R0),   OP(ADD_A_R1),   OP(ADD_A_R2),   OP(ADD_A_R3),   OP(ADD_A_R4),   OP(ADD_A_R5),   OP(ADD_A_R6),  OP(ADD_A_R7),    // 68-6F
    OP(ADC_A_XR0),  OP(ADC_A_XR1),  OP(JB_3),       OP(Illegal),    OP(CALL_3),     OP(ENT0_CLK),   OP(JF1),       OP(RR_A),        // 70-77
    OP(ADC_A_R0),   OP(ADC_A_R1),   OP(ADC_A_R2),   OP(ADC_A_R3),   OP(ADC_A_R4),   OP(ADC_A_R5),   OP(ADC_A_R6),  OP(ADC_A_R7),    // 78-7F
    OP(MOVX_A_XR0), OP(MOVX_A_XR1), OP(Illegal),    OP(RET),        OP(JMP_4),      OP(CLR_F0),     OP(JNI),       OP(Illegal),     // 80-87
    OP(ORL_BUS_N),  OP(ORL_P1_N),   OP(ORL_P2_N),   OP(Illegal),    OP(ORLD_P4_A),  OP(ORLD_P5_A),  OP(ORLD_P6_A), OP(ORLD_P7_A),   // 88-8F
    OP(MOVX_XR0_A), OP(MOVX_XR1_A), OP(JB_4),       OP(RETR),       OP(CALL_4),     OP(CPL_F0),     OP(JNZ),       OP(CLR_C),       // 90-97
    OP(ANL_BUS_N),  OP(ANL_P1_N),   OP(ANL_P2_N),   OP(Illegal),    OP(ANLD_P4_A),  OP(ANLD_P5_A),  OP(ANLD_P6_A), OP(ANLD_P7_A),   // 98-9F
    OP(MOV_XR0_A),  OP(MOV_XR1_A),  OP(Illegal),    OP(MOVP_A_XA),  OP(JMP_5),      OP(CLR_F1),     OP(Illegal),   OP(CPL_C),       // A0-A7
    OP(MOV_R0_A),   OP(MOV_R1_A),   OP(MOV_R2_A),   OP(MOV_R3_A),   OP(MOV_R4_A),   OP(MOV_R5_A),   OP(MOV_R6_A),  OP(MOV_R7_A),    // A8-AF
    OP(MOV_XR0_N),  OP(MOV_XR0_N),  OP(JB_5),       OP(JMPP_XA),    OP(CALL_5),     OP(CPL_F1),     OP(JF0),       OP(Illegal),     // B0-B7
    OP(MOV_R0_N),   OP(MOV_R1_N),   OP(MOV_R2_N),   OP(MOV_R3_N),   OP(MOV_R4_N),   OP(MOV_R5_N),   OP(MOV_R6_N),  OP(MOV_R7_N),    // B8-BF
    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(JMP_6),      OP(SEL_RB0),    OP(JZ),        OP(MOV_A_PSW),   // C0-C7
    OP(DEC_R0),     OP(DEC_R1),     OP(DEC_R2),     OP(DEC_R3),     OP(DEC_R4),     OP(DEC_R5),     OP(DEC_R6),    OP(DEC_R7),      // C8-CF
    OP(XRL_A_XR0),  OP(XRL_A_XR1),  OP(JB_6),       OP(XRL_A_N),    OP(CALL_6),     OP(SEL_RB1),    OP(Illegal),   OP(MOV_PSW_A),   // D0-D7
    OP(XRL_A_R0),   OP(XRL_A_R1),   OP(XRL_A_R2),   OP(XRL_A_R3),   OP(XRL_A_R4),   OP(XRL_A_R5),   OP(XRL_A_R6),  OP(XRL_A_R7),    // D8-DF
    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(MOVP3_A_XA), OP(JMP_7),      OP(SEL_MB0),    OP(JNC),       OP(RL_A),        // E0-E7
    OP(DJNZ_R0),    OP(DJNZ_R1),    OP(DJNZ_R2),    OP(DJNZ_R3),    OP(DJNZ_R4),    OP(DJNZ_R5),    OP(DJNZ_R6),   OP(DJNZ_R7),     // E8-EF
    OP(MOV_A_XR0),  OP(MOV_A_XR1),  OP(JB_7),       OP(Illegal),    OP(CALL_7),     OP(SEL_MB1),    OP(JC),        OP(RLC_A),       // F0-F7
    OP(MOV_A_R0),   OP(MOV_A_R1),   OP(MOV_A_R2),   OP(MOV_A_R3),   OP(MOV_A_R4),   OP(MOV_A_R5),   OP(MOV_A_R6),  OP(MOV_A_R7)     // F8-FF
};

const mcs48_cpuDevice::opExecute mcs48_cpuDevice::i8021_Opcodes[256] =
{
    OP(NOP),        OP(Illegal),    OP(Illegal),    OP(ADD_A_N),    OP(JMP_0),      OP(Illegal),    OP(Illegal),   OP(DEC_A),       // 00-07
    OP(IN_A_P0),    OP(IN_A_P1),    OP(IN_A_P2),    OP(Illegal),    OP(MOVD_A_P4),  OP(MOVD_A_P5),  OP(MOVD_A_P6), OP(MOVD_A_P7),   // 08-0F
    OP(INC_XR0),    OP(INC_XR1),    OP(Illegal),    OP(ADC_A_N),    OP(CALL_0),     OP(Illegal),    OP(JTF),       OP(INC_A),       // 10-17
    OP(INC_R0),     OP(INC_R1),     OP(INC_R2),     OP(INC_R3),     OP(INC_R4),     OP(INC_R5),     OP(INC_R6),    OP(INC_R7),      // 18-1F
    OP(XCH_A_XR0),  OP(XCH_A_XR1),  OP(Illegal),    OP(MOV_A_N),    OP(JMP_1),      OP(Illegal),    OP(Illegal),   OP(CLR_A),       // 20-27
    OP(XCH_A_R0),   OP(XCH_A_R1),   OP(XCH_A_R2),   OP(XCH_A_R3),   OP(XCH_A_R4),   OP(XCH_A_R5),   OP(XCH_A_R6),  OP(XCH_A_R7),    // 28-2F
    OP(XCHD_A_XR0), OP(XCHD_A_XR1), OP(Illegal),    OP(Illegal),    OP(CALL_1),     OP(Illegal),    OP(Illegal),   OP(CPL_A),       // 30-37
    OP(Illegal),    OP(OUTL_P1_A),  OP(OUTL_P2_A),  OP(Illegal),    OP(MOVD_P4_A),  OP(MOVD_P5_A),  OP(MOVD_P6_A), OP(MOVD_P7_A),   // 38-3F
    OP(ORL_A_XR0),  OP(ORL_A_XR1),  OP(MOV_A_T),    OP(ORL_A_N),    OP(JMP_2),      OP(STRT_CNT),   OP(JNT_1),     OP(SWAP_A),      // 40-47
    OP(ORL_A_R0),   OP(ORL_A_R1),   OP(ORL_A_R2),   OP(ORL_A_R3),   OP(ORL_A_R4),   OP(ORL_A_R5),   OP(ORL_A_R6),  OP(ORL_A_R7),    // 48-4F
    OP(ANL_A_XR0),  OP(ANL_A_XR1),  OP(Illegal),    OP(ANL_A_N),    OP(CALL_2),     OP(STRT_T),     OP(JT_1),      OP(DA_A),        // 50-57
    OP(ANL_A_R0),   OP(ANL_A_R1),   OP(ANL_A_R2),   OP(ANL_A_R3),   OP(ANL_A_R4),   OP(ANL_A_R5),   OP(ANL_A_R6),  OP(ANL_A_R7),    // 58-5F
    OP(ADD_A_XR0),  OP(ADD_A_XR1),  OP(MOV_T_A),    OP(Illegal),    OP(JMP_3),      OP(STOP_TCNT),  OP(Illegal),   OP(RRC_A),       // 60-67
    OP(ADD_A_R0),   OP(ADD_A_R1),   OP(ADD_A_R2),   OP(ADD_A_R3),   OP(ADD_A_R4),   OP(ADD_A_R5),   OP(ADD_A_R6),  OP(ADD_A_R7),    // 68-6F
    OP(ADC_A_XR0),  OP(ADC_A_XR1),  OP(Illegal),    OP(Illegal),    OP(CALL_3),     OP(Illegal),    OP(Illegal),   OP(RR_A),        // 70-77
    OP(ADC_A_R0),   OP(ADC_A_R1),   OP(ADC_A_R2),   OP(ADC_A_R3),   OP(ADC_A_R4),   OP(ADC_A_R5),   OP(ADC_A_R6),  OP(ADC_A_R7),    // 78-7F
    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(RET),        OP(JMP_4),      OP(Illegal),    OP(Illegal),   OP(Illegal),     // 80-87
    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(ORLD_P4_A),  OP(ORLD_P5_A),  OP(ORLD_P6_A), OP(ORLD_P7_A),   // 88-8F
    OP(OUTL_P0_A),  OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(CALL_4),     OP(Illegal),    OP(JNZ),       OP(CLR_C),       // 90-97
    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(ANLD_P4_A),  OP(ANLD_P5_A),  OP(ANLD_P6_A), OP(ANLD_P7_A),   // 98-9F
    OP(MOV_XR0_A),  OP(MOV_XR1_A),  OP(Illegal),    OP(MOVP_A_XA),  OP(JMP_5),      OP(Illegal),    OP(Illegal),   OP(CPL_C),       // A0-A7
    OP(MOV_R0_A),   OP(MOV_R1_A),   OP(MOV_R2_A),   OP(MOV_R3_A),   OP(MOV_R4_A),   OP(MOV_R5_A),   OP(MOV_R6_A),  OP(MOV_R7_A),    // A8-AF
    OP(MOV_XR0_N),  OP(MOV_XR0_N),  OP(Illegal),    OP(JMPP_XA),    OP(CALL_5),     OP(Illegal),    OP(Illegal),   OP(Illegal),     // B0-B7
    OP(MOV_R0_N),   OP(MOV_R1_N),   OP(MOV_R2_N),   OP(MOV_R3_N),   OP(MOV_R4_N),   OP(MOV_R5_N),   OP(MOV_R6_N),  OP(MOV_R7_N),    // B8-BF
    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(JMP_6),      OP(Illegal),    OP(JZ),        OP(Illegal),     // C0-C7
    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(Illegal),   OP(Illegal),     // C8-CF
    OP(XRL_A_XR0),  OP(XRL_A_XR1),  OP(Illegal),    OP(XRL_A_N),    OP(CALL_6),     OP(Illegal),    OP(Illegal),   OP(Illegal),     // D0-D7
    OP(XRL_A_R0),   OP(XRL_A_R1),   OP(XRL_A_R2),   OP(XRL_A_R3),   OP(XRL_A_R4),   OP(XRL_A_R5),   OP(XRL_A_R6),  OP(XRL_A_R7),    // D8-DF
    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(JMP_7),      OP(Illegal),    OP(JNC),       OP(RL_A),        // E0-E7
    OP(DJNZ_R0),    OP(DJNZ_R1),    OP(DJNZ_R2),    OP(DJNZ_R3),    OP(DJNZ_R4),    OP(DJNZ_R5),    OP(DJNZ_R6),   OP(DJNZ_R7),     // E8-EF
    OP(MOV_A_XR0),  OP(MOV_A_XR1),  OP(Illegal),    OP(Illegal),    OP(CALL_7),     OP(Illegal),    OP(JC),        OP(RLC_A),       // F0-F7
    OP(MOV_A_R0),   OP(MOV_A_R1),   OP(MOV_A_R2),   OP(MOV_A_R3),   OP(MOV_A_R4),   OP(MOV_A_R5),   OP(MOV_A_R6),  OP(MOV_A_R7)     // F8-FF
};

const mcs48_cpuDevice::opExecute mcs48_cpuDevice::i8022_Opcodes[256] =
{
    OP(NOP),        OP(Illegal),    OP(Illegal),    OP(ADD_A_N),    OP(JMP_0),      OP(EN_I),       OP(Illegal),   OP(DEC_A),       // 00-07
    OP(IN_A_P0),    OP(IN_A_P1),    OP(IN_A_P2),    OP(Illegal),    OP(MOVD_A_P4),  OP(MOVD_A_P5),  OP(MOVD_A_P6), OP(MOVD_A_P7),   // 08-0F
    OP(INC_XR0),    OP(INC_XR1),    OP(Illegal),    OP(ADC_A_N),    OP(CALL_0),     OP(DIS_I),      OP(JTF),       OP(INC_A),       // 10-17
    OP(INC_R0),     OP(INC_R1),     OP(INC_R2),     OP(INC_R3),     OP(INC_R4),     OP(INC_R5),     OP(INC_R6),    OP(INC_R7),      // 18-1F
    OP(XCH_A_XR0),  OP(XCH_A_XR1),  OP(Illegal),    OP(MOV_A_N),    OP(JMP_1),      OP(EN_TCNTI),   OP(JNT_0),     OP(CLR_A),       // 20-27
    OP(XCH_A_R0),   OP(XCH_A_R1),   OP(XCH_A_R2),   OP(XCH_A_R3),   OP(XCH_A_R4),   OP(XCH_A_R5),   OP(XCH_A_R6),  OP(XCH_A_R7),    // 28-2F
    OP(XCHD_A_XR0), OP(XCHD_A_XR1), OP(Illegal),    OP(Illegal),    OP(CALL_1),     OP(DIS_TCNTI),  OP(JT_0),      OP(CPL_A),       // 30-37
    OP(Illegal),    OP(OUTL_P1_A),  OP(OUTL_P2_A),  OP(Illegal),    OP(MOVD_P4_A),  OP(MOVD_P5_A),  OP(MOVD_P6_A), OP(MOVD_P7_A),   // 38-3F
    OP(ORL_A_XR0),  OP(ORL_A_XR1),  OP(MOV_A_T),    OP(ORL_A_N),    OP(JMP_2),      OP(STRT_CNT),   OP(JNT_1),     OP(SWAP_A),      // 40-47
    OP(ORL_A_R0),   OP(ORL_A_R1),   OP(ORL_A_R2),   OP(ORL_A_R3),   OP(ORL_A_R4),   OP(ORL_A_R5),   OP(ORL_A_R6),  OP(ORL_A_R7),    // 48-4F
    OP(ANL_A_XR0),  OP(ANL_A_XR1),  OP(Illegal),    OP(ANL_A_N),    OP(CALL_2),     OP(STRT_T),     OP(JT_1),      OP(DA_A),        // 50-57
    OP(ANL_A_R0),   OP(ANL_A_R1),   OP(ANL_A_R2),   OP(ANL_A_R3),   OP(ANL_A_R4),   OP(ANL_A_R5),   OP(ANL_A_R6),  OP(ANL_A_R7),    // 58-5F
    OP(ADD_A_XR0),  OP(ADD_A_XR1),  OP(MOV_T_A),    OP(Illegal),    OP(JMP_3),      OP(STOP_TCNT),  OP(Illegal),   OP(RRC_A),       // 60-67
    OP(ADD_A_R0),   OP(ADD_A_R1),   OP(ADD_A_R2),   OP(ADD_A_R3),   OP(ADD_A_R4),   OP(ADD_A_R5),   OP(ADD_A_R6),  OP(ADD_A_R7),    // 68-6F
    OP(ADC_A_XR0),  OP(ADC_A_XR1),  OP(Illegal),    OP(Illegal),    OP(CALL_3),     OP(Illegal),    OP(Illegal),   OP(RR_A),        // 70-77
    OP(ADC_A_R0),   OP(ADC_A_R1),   OP(ADC_A_R2),   OP(ADC_A_R3),   OP(ADC_A_R4),   OP(ADC_A_R5),   OP(ADC_A_R6),  OP(ADC_A_R7),    // 78-7F
    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(RET),        OP(JMP_4),      OP(Illegal),    OP(Illegal),   OP(Illegal),     // 80-87
    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(ORLD_P4_A),  OP(ORLD_P5_A),  OP(ORLD_P6_A), OP(ORLD_P7_A),   // 88-8F
    OP(OUTL_P0_A),  OP(Illegal),    OP(Illegal),    OP(RETR),       OP(CALL_4),     OP(Illegal),    OP(JNZ),       OP(CLR_C),       // 90-97
    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(ANLD_P4_A),  OP(ANLD_P5_A),  OP(ANLD_P6_A), OP(ANLD_P7_A),   // 98-9F
    OP(MOV_XR0_A),  OP(MOV_XR1_A),  OP(Illegal),    OP(MOVP_A_XA),  OP(JMP_5),      OP(Illegal),    OP(Illegal),   OP(CPL_C),       // A0-A7
    OP(MOV_R0_A),   OP(MOV_R1_A),   OP(MOV_R2_A),   OP(MOV_R3_A),   OP(MOV_R4_A),   OP(MOV_R5_A),   OP(MOV_R6_A),  OP(MOV_R7_A),    // A8-AF
    OP(MOV_XR0_N),  OP(MOV_XR0_N),  OP(Illegal),    OP(JMPP_XA),    OP(CALL_5),     OP(Illegal),    OP(Illegal),   OP(Illegal),     // B0-B7
    OP(MOV_R0_N),   OP(MOV_R1_N),   OP(MOV_R2_N),   OP(MOV_R3_N),   OP(MOV_R4_N),   OP(MOV_R5_N),   OP(MOV_R6_N),  OP(MOV_R7_N),    // B8-BF
    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(JMP_6),      OP(Illegal),    OP(JZ),        OP(Illegal),     // C0-C7
    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(Illegal),   OP(Illegal),     // C8-CF
    OP(XRL_A_XR0),  OP(XRL_A_XR1),  OP(Illegal),    OP(XRL_A_N),    OP(CALL_6),     OP(Illegal),    OP(Illegal),   OP(Illegal),     // D0-D7
    OP(XRL_A_R0),   OP(XRL_A_R1),   OP(XRL_A_R2),   OP(XRL_A_R3),   OP(XRL_A_R4),   OP(XRL_A_R5),   OP(XRL_A_R6),  OP(XRL_A_R7),    // D8-DF
    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(JMP_7),      OP(Illegal),    OP(JNC),       OP(RL_A),        // E0-E7
    OP(DJNZ_R0),    OP(DJNZ_R1),    OP(DJNZ_R2),    OP(DJNZ_R3),    OP(DJNZ_R4),    OP(DJNZ_R5),    OP(DJNZ_R6),   OP(DJNZ_R7),     // E8-EF
    OP(MOV_A_XR0),  OP(MOV_A_XR1),  OP(Illegal),    OP(Illegal),    OP(CALL_7),     OP(Illegal),    OP(JC),        OP(RLC_A),       // F0-F7
    OP(MOV_A_R0),   OP(MOV_A_R1),   OP(MOV_A_R2),   OP(MOV_A_R3),   OP(MOV_A_R4),   OP(MOV_A_R5),   OP(MOV_A_R6),  OP(MOV_A_R7)     // F8-FF
};

const mcs48_cpuDevice::opExecute mcs48_cpuDevice::upi41_Opcodes[256] =
{
    OP(NOP),        OP(Illegal),    OP(OUT_DBB_A),  OP(ADD_A_N),    OP(JMP_0),      OP(EN_I),       OP(Illegal),   OP(DEC_A),       // 00-07
    OP(Illegal),    OP(IN_A_P1),    OP(IN_A_P2),    OP(Illegal),    OP(MOVD_A_P4),  OP(MOVD_A_P5),  OP(MOVD_A_P6), OP(MOVD_A_P7),   // 08-0F
    OP(INC_XR0),    OP(INC_XR1),    OP(JB_0),       OP(ADC_A_N),    OP(CALL_0),     OP(DIS_I),      OP(JTF),       OP(INC_A),       // 10-17
    OP(INC_R0),     OP(INC_R1),     OP(INC_R2),     OP(INC_R3),     OP(INC_R4),     OP(INC_R5),     OP(INC_R6),    OP(INC_R7),      // 18-1F
    OP(XCH_A_XR0),  OP(XCH_A_XR1),  OP(IN_A_DBB),   OP(MOV_A_N),    OP(JMP_1),      OP(EN_TCNTI),   OP(JNT_0),     OP(CLR_A),       // 20-27
    OP(XCH_A_R0),   OP(XCH_A_R1),   OP(XCH_A_R2),   OP(XCH_A_R3),   OP(XCH_A_R4),   OP(XCH_A_R5),   OP(XCH_A_R6),  OP(XCH_A_R7),    // 28-2F
    OP(XCHD_A_XR0), OP(XCHD_A_XR1), OP(JB_1),       OP(Illegal),    OP(CALL_1),     OP(DIS_TCNTI),  OP(JT_0),      OP(CPL_A),       // 30-37
    OP(Illegal),    OP(OUTL_P1_A),  OP(OUTL_P2_A),  OP(Illegal),    OP(MOVD_P4_A),  OP(MOVD_P5_A),  OP(MOVD_P6_A), OP(MOVD_P7_A),   // 38-3F
    OP(ORL_A_XR0),  OP(ORL_A_XR1),  OP(MOV_A_T),    OP(ORL_A_N),    OP(JMP_2),      OP(STRT_CNT),   OP(JNT_1),     OP(SWAP_A),      // 40-47
    OP(ORL_A_R0),   OP(ORL_A_R1),   OP(ORL_A_R2),   OP(ORL_A_R3),   OP(ORL_A_R4),   OP(ORL_A_R5),   OP(ORL_A_R6),  OP(ORL_A_R7),    // 48-4F
    OP(ANL_A_XR0),  OP(ANL_A_XR1),  OP(JB_2),       OP(ANL_A_N),    OP(CALL_2),     OP(STRT_T),     OP(JT_1),      OP(DA_A),        // 50-57
    OP(ANL_A_R0),   OP(ANL_A_R1),   OP(ANL_A_R2),   OP(ANL_A_R3),   OP(ANL_A_R4),   OP(ANL_A_R5),   OP(ANL_A_R6),  OP(ANL_A_R7),    // 58-5F
    OP(ADD_A_XR0),  OP(ADD_A_XR1),  OP(MOV_T_A),    OP(Illegal),    OP(JMP_3),      OP(STOP_TCNT),  OP(Illegal),   OP(RRC_A),       // 60-67
    OP(ADD_A_R0),   OP(ADD_A_R1),   OP(ADD_A_R2),   OP(ADD_A_R3),   OP(ADD_A_R4),   OP(ADD_A_R5),   OP(ADD_A_R6),  OP(ADD_A_R7),    // 68-6F
    OP(ADC_A_XR0),  OP(ADC_A_XR1),  OP(JB_3),       OP(Illegal),    OP(CALL_3),     OP(Illegal),    OP(JF1),       OP(RR_A),        // 70-77
    OP(ADC_A_R0),   OP(ADC_A_R1),   OP(ADC_A_R2),   OP(ADC_A_R3),   OP(ADC_A_R4),   OP(ADC_A_R5),   OP(ADC_A_R6),  OP(ADC_A_R7),    // 78-7F
    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(RET),        OP(JMP_4),      OP(CLR_F0),     OP(JOBF),      OP(Illegal),     // 80-87
    OP(Illegal),    OP(ORL_P1_N),   OP(ORL_P2_N),   OP(Illegal),    OP(ORLD_P4_A),  OP(ORLD_P5_A),  OP(ORLD_P6_A), OP(ORLD_P7_A),   // 88-8F
    OP(MOV_STS_A),  OP(Illegal),    OP(JB_4),       OP(RETR),       OP(CALL_4),     OP(CPL_F0),     OP(JNZ),       OP(CLR_C),       // 90-97
    OP(Illegal),    OP(ANL_P1_N),   OP(ANL_P2_N),   OP(Illegal),    OP(ANLD_P4_A),  OP(ANLD_P5_A),  OP(ANLD_P6_A), OP(ANLD_P7_A),   // 98-9F
    OP(MOV_XR0_A),  OP(MOV_XR1_A),  OP(Illegal),    OP(MOVP_A_XA),  OP(JMP_5),      OP(CLR_F1),     OP(Illegal),   OP(CPL_C),       // A0-A7
    OP(MOV_R0_A),   OP(MOV_R1_A),   OP(MOV_R2_A),   OP(MOV_R3_A),   OP(MOV_R4_A),   OP(MOV_R5_A),   OP(MOV_R6_A),  OP(MOV_R7_A),    // A8-AF
    OP(MOV_XR0_N),  OP(MOV_XR0_N),  OP(JB_5),       OP(JMPP_XA),    OP(CALL_5),     OP(CPL_F1),     OP(JF0),       OP(Illegal),     // B0-B7
    OP(MOV_R0_N),   OP(MOV_R1_N),   OP(MOV_R2_N),   OP(MOV_R3_N),   OP(MOV_R4_N),   OP(MOV_R5_N),   OP(MOV_R6_N),  OP(MOV_R7_N),    // B8-BF
    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(JMP_6),      OP(SEL_RB0),    OP(JZ),        OP(MOV_A_PSW),   // C0-C7
    OP(DEC_R0),     OP(DEC_R1),     OP(DEC_R2),     OP(DEC_R3),     OP(DEC_R4),     OP(DEC_R5),     OP(DEC_R6),    OP(DEC_R7),      // C8-CF
    OP(XRL_A_XR0),  OP(XRL_A_XR1),  OP(JB_6),       OP(XRL_A_N),    OP(CALL_6),     OP(SEL_RB1),    OP(JNIBF),     OP(MOV_PSW_A),   // D0-D7
    OP(XRL_A_R0),   OP(XRL_A_R1),   OP(XRL_A_R2),   OP(XRL_A_R3),   OP(XRL_A_R4),   OP(XRL_A_R5),   OP(XRL_A_R6),  OP(XRL_A_R7),    // D8-DF
    OP(Illegal),    OP(Illegal),    OP(Illegal),    OP(MOVP3_A_XA), OP(JMP_7),      OP(EN_DMA),     OP(JNC),       OP(RL_A),        // E0-E7
    OP(DJNZ_R0),    OP(DJNZ_R1),    OP(DJNZ_R2),    OP(DJNZ_R3),    OP(DJNZ_R4),    OP(DJNZ_R5),    OP(DJNZ_R6),   OP(DJNZ_R7),     // E8-EF
    OP(MOV_A_XR0),  OP(MOV_A_XR1),  OP(JB_7),       OP(Illegal),    OP(CALL_7),     OP(EN_FLAGS),   OP(JC),        OP(RLC_A),       // F0-F7
    OP(MOV_A_R0),   OP(MOV_A_R1),   OP(MOV_A_R2),   OP(MOV_A_R3),   OP(MOV_A_R4),   OP(MOV_A_R5),   OP(MOV_A_R6),  OP(MOV_A_R7)     // F8-FF
};
