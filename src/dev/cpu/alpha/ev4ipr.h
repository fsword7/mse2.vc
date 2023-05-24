// ev4ipr.h - Alpha EV4 Processor - Internal Processor Registers
//
// Author:  Tim Stark
// Date:    May 21, 2023

#pragma once

// IBOX IPR registers table
#define IPR4n_TB_TAG          0x20  // (W)      PALmode only
#define IPR4n_ITB_PTE         0x21  // (R/W)    PALmode only
#define IPR4n_ICCSR           0x22  // (R/W) 
#define IPR4n_ITB_PTE_TEMP    0x23  // (R)      PALmode only
#define IPR4n_EXC_ADDR        0x24  // (R/W)
#define IPR4n_SL_RCV          0x25  // (R)
#define IPR4n_ITBZAP          0x26  // (W)      PALmode only
#define IPR4n_ITBASM          0x27  // (W)      PALmode only
#define IPR4n_ITBIS           0x28  // (W)      PALmode only
#define IPR4n_PS              0x29  // (R/W)
#define IPR4n_EXC_SUM         0x2A  // (R/W)
#define IPR4n_PAL_BASE        0x2B  // (R/W)
#define IPR4n_HIRR            0x2C  // (R)
#define IPR4n_SIRR            0x2D  // (R/W)
#define IPR4n_ASTRR           0x2E  // (R/W)
#define IPR4n_HIER            0x30  // (R/W)
#define IPR4n_SIER            0x31  // (R/W)
#define IPR4n_ASTER           0x32  // (R/W)
#define IPR4n_SL_CLR          0x33  // (W)
#define IPR4n_SL_XMIT         0x36  // (W)

// ABOX IPR registers table
#define IPR4n_TB_CTL          0x40  // (W)
#define IPR4n_DTB_PTE         0x42  // (R/W)
#define IPR4n_DTB_PTE_TEMP    0x43  // (R)
#define IPR4n_MM_CSR          0x44  // (R)
#define IPR4n_VA              0x45  // (R)
#define IPR4n_DTBZAP          0x46  // (W)
#define IPR4n_DTBASM          0x47  // (W)
#define IPR4n_DTBIS           0x48  // (W)
#define IPR4n_BIU_ADDR        0x49  // (R)
#define IPR4n_BIU_STAT        0x4A  // (R)
#define IPR4n_DC_STAT         0x4C  // (R)      21064 only
#define IPR4n_C_STAT          0x4C  // (R)      21064A only
#define IPR4n_FILL_ADDR       0x4D  // (R)
#define IPR4n_ABOX_CTL        0x4E  // (W)
#define IPR4n_ALT_MODE        0x4F  // (W)
#define IPR4n_CC              0x50  // (W)
#define IPR4n_CC_CTL          0x51  // (W)
#define IPR4n_BIU_CTL         0x52  // (W)
#define IPR4n_FILL_SYNDROME   0x53  // (R)
#define IPR4n_BC_TAG          0x54  // (R)
#define IPR4n_FLUSH_IC        0x55  // (W)
#define IPR4n_FLUSH_IC_ASM    0x57  // (W)

// PAL IPR registers table
#define IPR4n_PAL_TEMP0       0x80  // (R/W) PALtemp 0 register
#define IPR4n_PAL_TEMP1       0x81  // (R/W) PALtemp 1 register
#define IPR4n_PAL_TEMP2       0x82  // (R/W) PALtemp 2 register
#define IPR4n_PAL_TEMP3       0x83  // (R/W) PALtemp 3 register
#define IPR4n_PAL_TEMP4       0x84  // (R/W) PALtemp 4 register
#define IPR4n_PAL_TEMP5       0x85  // (R/W) PALtemp 5 register
#define IPR4n_PAL_TEMP6       0x86  // (R/W) PALtemp 6 register
#define IPR4n_PAL_TEMP7       0x87  // (R/W) PALtemp 7 register
#define IPR4n_PAL_TEMP8       0x88  // (R/W) PALtemp 8 register
#define IPR4n_PAL_TEMP9       0x89  // (R/W) PALtemp 9 register
#define IPR4n_PAL_TEMP10      0x8A  // (R/W) PALtemp 10 register
#define IPR4n_PAL_TEMP11      0x8B  // (R/W) PALtemp 11 register
#define IPR4n_PAL_TEMP12      0x8C  // (R/W) PALtemp 12 register
#define IPR4n_PAL_TEMP13      0x8D  // (R/W) PALtemp 13 register
#define IPR4n_PAL_TEMP14      0x8E  // (R/W) PALtemp 14 register
#define IPR4n_PAL_TEMP15      0x8F  // (R/W) PALtemp 15 register
#define IPR4n_PAL_TEMP16      0x90  // (R/W) PALtemp 16 register
#define IPR4n_PAL_TEMP17      0x91  // (R/W) PALtemp 17 register
#define IPR4n_PAL_TEMP18      0x92  // (R/W) PALtemp 18 register
#define IPR4n_PAL_TEMP19      0x93  // (R/W) PALtemp 19 register
#define IPR4n_PAL_TEMP20      0x94  // (R/W) PALtemp 20 register
#define IPR4n_PAL_TEMP21      0x95  // (R/W) PALtemp 21 register
#define IPR4n_PAL_TEMP22      0x96  // (R/W) PALtemp 22 register
#define IPR4n_PAL_TEMP23      0x97  // (R/W) PALtemp 23 register
#define IPR4n_PAL_TEMP24      0x98  // (R/W) PALtemp 24 register
#define IPR4n_PAL_TEMP25      0x99  // (R/W) PALtemp 25 register
#define IPR4n_PAL_TEMP26      0x9A  // (R/W) PALtemp 26 register
#define IPR4n_PAL_TEMP27      0x9B  // (R/W) PALtemp 27 register
#define IPR4n_PAL_TEMP28      0x9C  // (R/W) PALtemp 28 register
#define IPR4n_PAL_TEMP29      0x9D  // (R/W) PALtemp 29 register
#define IPR4n_PAL_TEMP30      0x9E  // (R/W) PALtemp 30 register
#define IPR4n_PAL_TEMP31      0x9F  // (R/W) PALtemp 31 register
