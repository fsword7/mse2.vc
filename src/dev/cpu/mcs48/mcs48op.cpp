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

const mcs48_cpuDevice::opExecute_t mcs48_cpuDevice::mcs48_Opcodes[256] =
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

const mcs48_cpuDevice::opExecute_t mcs48_cpuDevice::i8021_Opcodes[256] =
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

const mcs48_cpuDevice::opExecute_t mcs48_cpuDevice::i8022_Opcodes[256] =
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

const mcs48_cpuDevice::opExecute_t mcs48_cpuDevice::upi41_Opcodes[256] =
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

#define INST(opName, opReg, opType, opCode, opMask, cycle, opFlags) \
	{ opName, opReg, opType, opCode, opMask, opFlags, cycle }


#define A2X     ARCH_I802X
#define U41     ARCH_UPI41
#define A4X     ARCH_I804X
#define AED     ARCH_EXTBUS
#define AMB     ARCH_MB


const mcs48_cpuDevice::mcs48op_t mcs48_cpuDevice::opTable[] =
{
	INST("ADD",	  "A,R%d",      OPR_REG,           0x68, 0x07, 1, A2X|A4X|U41),
	INST("ADD",   "A,@R%d",     OPR_REG,           0x60, 0x01, 1, A2X|A4X|U41),
	INST("ADD",   "A,#%02X",    OPR_LIT,           0x03, 0x00, 2, A2X|A4X|U41),
	INST("ADDC",  "A,R%d",      OPR_REG,           0x78, 0x07, 1, A2X|A4X|U41),
	INST("ADDC",  "A,@R%d",     OPR_REG,           0x70, 0x01, 1, A2X|A4X|U41),
	INST("ADDC",  "A,#%02X",    OPR_LIT,           0x13, 0x00, 2, A2X|A4X|U41),
	INST("ANL",   "A,R%d",      OPR_REG,           0x58, 0x07, 1, A2X|A4X|U41),
	INST("ANL",   "A,@R%d",     OPR_REG,           0x50, 0x01, 1, A2X|A4X|U41),
	INST("ANL",   "A,#%02X",    OPR_LIT,           0x53, 0x00, 2, A2X|A4X|U41),
	INST("ANL",   "P%d,#%02X",  OPR_REG|OPR_LIT,   0x98, 0x03, 2, A2X|A4X|U41),
	INST("ANL",   "BUS,#%02X",  OPR_LIT,           0x98, 0x00, 2, A2X|A4X|U41),
	INST("ANLD",  "P%d,A",      OPR_REG,           0x9C, 0x03, 2, A2X|A4X|U41),
	INST("CALL",  "%03X",       OPR_ADDR3,         0x14, 0xE0, 2, A2X|A4X|U41),
	INST("CLR",   "A",          OPR_NOPE,          0x27, 0x00, 1, A2X|A4X|U41),
	INST("CLR",   "C",          OPR_NOPE,          0x97, 0x00, 1, A2X|A4X|U41),
	INST("CLR",   "F0",         OPR_NOPE,          0x85, 0x00, 1, A2X|A4X|U41),
	INST("CLR",   "F1",         OPR_NOPE,          0xA5, 0x00, 1, A2X|A4X|U41),
	INST("CPL",   "A",          OPR_NOPE,          0x37, 0x00, 1, A2X|A4X|U41),
	INST("CPL",   "C",          OPR_NOPE,          0xA7, 0x00, 1, A2X|A4X|U41),
	INST("CPL",   "F0",         OPR_NOPE,          0x95, 0x00, 1, A2X|A4X|U41),
	INST("CPL",   "F1",         OPR_NOPE,          0xB5, 0x00, 1, A2X|A4X|U41),
	INST("DA",    "A",          OPR_NOPE,          0x57, 0x00, 1, A2X|A4X|U41),
	INST("DEC",   "A",          OPR_NOPE,          0x07, 0x00, 1, A2X|A4X|U41),
	INST("DEC",   "R%d",        OPR_REG,           0xC8, 0x07, 1, A2X|A4X|U41),
	INST("DIS",   "I",          OPR_NOPE,          0x15, 0x00, 1, A2X|A4X|U41),
	INST("DIS",   "TCNTI",      OPR_NOPE,          0x35, 0x00, 1, A2X|A4X|U41),
	INST("DJNZ",  "R%d,%03X",   OPR_REG|OPR_ADDR2, 0xE8, 0x07, 2, A2X|A4X|U41),
	INST("EN",    "I",          OPR_NOPE,          0x05, 0x00, 1, A2X|A4X|U41),
	INST("EN",    "TCNTI",      OPR_NOPE,          0x25, 0x00, 1, A2X|A4X|U41),
	INST("ENT0",  "CLK",        OPR_NOPE,          0x75, 0x00, 1, A2X|A4X|U41),
	INST("IN",    "A,P%d",      OPR_REG,           0x08, 0x03, 2, A2X|A4X|U41),
	INST("INC",   "A",          OPR_NOPE,          0x17, 0x00, 1, A2X|A4X|U41),
	INST("INC",   "R%d",        OPR_REG,           0x18, 0x07, 1, A2X|A4X|U41),
	INST("INC",   "@R%d",       OPR_REG,           0x10, 0x01, 1, A2X|A4X|U41),
	INST("IN",    "A,P0",       OPR_NOPE,          0x08, 0x00, 2, A2X),
    INST("IN",    "A,DBB",      OPR_NOPE,          0x22, 0x00, 2, U41),
	INST("INS",   "A,BUS",      OPR_NOPE,          0x08, 0x00, 2, A4X),
	INST("JB0",   "%03X",       OPR_ADDR2,         0x12, 0x00, 2, A4X|U41),
	INST("JB1",   "%03X",       OPR_ADDR2,         0x32, 0x00, 2, A4X|U41),
	INST("JB2",   "%03X",       OPR_ADDR2,         0x52, 0x00, 2, A4X|U41),
	INST("JB3",   "%03X",       OPR_ADDR2,         0x72, 0x00, 2, A4X|U41),
	INST("JB4",   "%03X",       OPR_ADDR2,         0x92, 0x00, 2, A4X|U41),
	INST("JB5",   "%03X",       OPR_ADDR2,         0xB2, 0x00, 2, A4X|U41),
	INST("JB6",   "%03X",       OPR_ADDR2,         0xD2, 0x00, 2, A4X|U41),
	INST("JB7",   "%03X",       OPR_ADDR2,         0xF2, 0x00, 2, A4X|U41),
	INST("JC",    "%03X",       OPR_ADDR2,         0xF6, 0x00, 2, A2X|A4X|U41),
	INST("JF0",   "%03X",       OPR_ADDR2,         0xB6, 0x00, 2, A4X|U41),
	INST("JF1",   "%03X",       OPR_ADDR2,         0x76, 0x00, 2, A4X|U41),
	INST("JMP",   "%03X",       OPR_ADDR3,         0x04, 0xE0, 2, A2X|A4X|U41),
	INST("JMPP",  "@A",         OPR_NOPE,          0xB3, 0x00, 2, A2X|A4X|U41),
	INST("JNC",   "%03X",       OPR_ADDR2,         0xE6, 0x00, 2, A2X|A4X|U41),
	INST("JNI",   "%03X",       OPR_ADDR2,         0x86, 0x00, 2, A4X|U41),
	INST("JNT0",  "%03X",       OPR_ADDR2,         0x26, 0x00, 2, A4X|U41),
	INST("JNT1",  "%03X",       OPR_ADDR2,         0x46, 0x00, 2, A2X|A4X|U41),
	INST("JNZ",   "%03X",       OPR_ADDR2,         0x96, 0x00, 2, A2X|A4X|U41),
	INST("JTF",   "%03X",       OPR_ADDR2,         0x16, 0x00, 2, A2X|A4X|U41),
	INST("JT0",   "%03X",       OPR_ADDR2,         0x36, 0x00, 2, A4X|U41),
	INST("JT1",   "%03X",       OPR_ADDR2,         0x56, 0x00, 2, A2X|A4X|U41),
	INST("JZ",    "%03X",       OPR_ADDR2,         0xC6, 0x00, 2, A2X|A4X|U41),
	INST("MOV",   "A,#%02X",    OPR_LIT,           0x23, 0x00, 2, A2X|A4X|U41),
	INST("MOV",   "A,PSW",      OPR_NOPE,          0xC7, 0x00, 1, A2X|A4X|U41),
	INST("MOV",   "A,R%d",      OPR_REG,           0xF8, 0x07, 1, A2X|A4X|U41),
	INST("MOV",   "A,@R%d",     OPR_REG,           0xF0, 0x01, 1, A2X|A4X|U41),
	INST("MOV",   "A,T",        OPR_NOPE,          0x42, 0x00, 1, A2X|A4X|U41),
	INST("MOV",   "PSW,A",      OPR_NOPE,          0xD7, 0x00, 1, A2X|A4X|U41),
	INST("MOV",   "R%d,A",      OPR_REG,           0xA8, 0x07, 1, A2X|A4X|U41),
	INST("MOV",   "R%d,#%02X",  OPR_REG|OPR_LIT,   0xB8, 0x07, 2, A2X|A4X|U41),
	INST("MOV",   "@R%d,A",     OPR_REG,           0xA0, 0x01, 1, A2X|A4X|U41),
	INST("MOV",   "@R%d,#%02X", OPR_REG|OPR_LIT,   0xB0, 0x01, 2, A2X|A4X|U41),
	INST("MOV",   "T,A",        OPR_NOPE,          0x62, 0x00, 1, A2X|A4X|U41),
	INST("MOVD",  "A,P%d",      OPR_PORT,          0x0C, 0x03, 2, A2X|A4X|U41),
	INST("MOVD",  "P%d,A",      OPR_PORT,          0x3C, 0x03, 2, A2X|A4X|U41),
	INST("MOVP",  "A,@A",       OPR_NOPE,          0xA3, 0x00, 2, A2X|A4X|U41),
	INST("MOVP3", "A,@A",       OPR_NOPE,          0xE3, 0x00, 2, A2X|A4X|U41),
	INST("MOVX",  "A,@R%d",     OPR_REG,           0x80, 0x01, 2, AED),
	INST("MOVX",  "@R%d,A",     OPR_REG,           0x90, 0x01, 2, AED),
	INST("NOP",   "",           OPR_NOPE,          0x00, 0x00, 1, A2X|A4X|U41),
	INST("ORL",   "A,R%d",      OPR_REG,           0x48, 0x07, 1, A2X|A4X|U41),
	INST("ORL",   "A,@R%d",     OPR_REG,           0x40, 0x01, 1, A2X|A4X|U41),
	INST("ORL",   "A,#%02X",    OPR_LIT,           0x43, 0x00, 2, A2X|A4X|U41),
	INST("ORL",   "BUS,#%02X",  OPR_LIT,           0x88, 0x00, 2, A4X|U41),
	INST("ORL",   "P%d,#%02X",  OPR_REG|OPR_LIT,   0x88, 0x03, 2, A2X|A4X|U41),
	INST("ORLD",  "P%d,A",      OPR_REG,           0x8C, 0x03, 2, A2X|A4X|U41),
    INST("OUT",   "DBB,A",      OPR_NOPE,          0x02, 0x00, 2, U41),
	INST("OUTL",  "P0,A",       OPR_NOPE,          0x90, 0x00, 1, A2X),
	INST("OUTL",  "BUS,A",      OPR_NOPE,          0x02, 0x00, 2, A4X),
	INST("OUTL",  "P%d,A",      OPR_REG,           0x38, 0x03, 2, A2X|A4X|U41),
	INST("RET",   "",           OPR_NOPE,          0x83, 0x00, 2, A2X|A4X|U41),
	INST("RETI",  "",           OPR_NOPE,          0x93, 0x00, 2, A2X), // Only 8022
	INST("RETR",  "",           OPR_NOPE,          0x93, 0x00, 2, A4X|U41),
	INST("RL",    "A",          OPR_NOPE,          0xE7, 0x00, 1, A2X|A4X|U41),
	INST("RLC",   "A",          OPR_NOPE,          0xF7, 0x00, 1, A2X|A4X|U41),
	INST("RR",    "A",          OPR_NOPE,          0x77, 0x00, 1, A2X|A4X|U41),
	INST("RRC",   "A",          OPR_NOPE,          0x67, 0x00, 1, A2X|A4X|U41),
	INST("SEL",   "MB0",        OPR_NOPE,          0xE5, 0x00, 1, A4X|U41),
	INST("SEL",   "MB1",        OPR_NOPE,          0xF5, 0x00, 1, A4X|U41),
	INST("SEL",   "RB0",        OPR_NOPE,          0xC5, 0x00, 1, A4X|U41),
	INST("SEL",   "RB1",        OPR_NOPE,          0xD5, 0x00, 1, A4X|U41),
	INST("STOP",  "TCNT",       OPR_NOPE,          0x65, 0x00, 1, A2X|A4X|U41),
	INST("STRT",  "CNT",        OPR_NOPE,          0x45, 0x00, 1, A2X|A4X|U41),
	INST("STRT",  "T",          OPR_NOPE,          0x55, 0x00, 1, A2X|A4X|U41),
	INST("SWAP",  "A",          OPR_NOPE,          0x47, 0x00, 1, A2X|A4X|U41),
	INST("XCH",   "A,R%d",      OPR_REG,           0x28, 0x07, 1, A2X|A4X|U41),
	INST("XCH",   "A,@R%d",     OPR_REG,           0x20, 0x01, 1, A2X|A4X|U41),
	INST("XCHD",  "A,@R%d",     OPR_REG,           0x30, 0x01, 1, A2X|A4X|U41),
	INST("XRL",   "A,R%d",      OPR_REG,           0xD8, 0x07, 1, A2X|A4X|U41),
	INST("XRL",   "A,@R%d",     OPR_REG,           0xD0, 0x01, 1, A2X|A4X|U41),
	INST("XRL",   "A,#%02X",    OPR_LIT,           0xD3, 0x00, 2, A2X|A4X|U41),

	// Null terminator
	nullptr
};
