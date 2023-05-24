// ev5ipr.h - Alpha EV5 Processor - Internal Processor Registers
//
// Author:  Tim Stark
// Date:    May 22, 2023

#pragma once

// IPR registers table
#define IPR5n_ISR               0x100   // (R)
#define IPR5n_ITB_TAG           0x101   // (W)
#define IPR5n_PTE               0x102   // (R/W)
#define IPR5n_ASN               0x103   // (R/W)
#define IPR5n_PTE_TEMP          0x104   // (R)
#define IPR5n_IA                0x105   // (W)
#define IPR5n_ITB_IAP           0x106   // (W)
#define IPR5n_ITB_IS            0x107   // (W)
#define IPR5n_SIRR              0x108   // (R/W)
#define IPR5n_ASTRR             0x109   // (R/W)
#define IPR5n_ASTER             0x10A   // (R/W)
#define IPR5n_EXC_ADDR          0x10B   // (R/W)
#define IPR5n_EXC_SUM           0x10C   // (R/W0C)
#define IPR5n_EXC_MASK          0x10D   // (R)
#define IPR5n_PAL_BASE          0x10E   // (R/W)
#define IPR5n_ICM               0x10F   // (R/W)
#define IPR5n_IPLR              0x110   // (R/W)
#define IPR5n_INTID             0x111   // (R)
#define IPR5n_IFAULT_VA_FORM    0x112   // (R)
#define IPR5n_IVPTBR            0x113   // (R/W)
#define IPR5n_HWINT_CLR         0x115   // (W)
#define IPR5n_SL_XMIT           0x116   // (W)
#define IPR5n_SL_RCV            0x117   // (R)
#define IPR5n_ICSR              0x118   // (R/W)
#define IPR5n_IC_FLUSH_CTL      0x119   // (W)
#define IPR5n_ICPERR_STAT       0x11A   // (R/W1C)
#define IPR5n_PMCTR             0x11C   // (R/W)

// PAL IPR registers table
#define IPR5n_PAL_TEMP0         0x140   // (R/W) PALtemp 0 register
#define IPR5n_PAL_TEMP1         0x141   // (R/W) PALtemp 1 register
#define IPR5n_PAL_TEMP2         0x142   // (R/W) PALtemp 2 register
#define IPR5n_PAL_TEMP3         0x143   // (R/W) PALtemp 3 register
#define IPR5n_PAL_TEMP4         0x144   // (R/W) PALtemp 4 register
#define IPR5n_PAL_TEMP5         0x145   // (R/W) PALtemp 5 register
#define IPR5n_PAL_TEMP6         0x146   // (R/W) PALtemp 6 register
#define IPR5n_PAL_TEMP7         0x167   // (R/W) PALtemp 7 register
#define IPR5n_PAL_TEMP8         0x148   // (R/W) PALtemp 8 register
#define IPR5n_PAL_TEMP9         0x149   // (R/W) PALtemp 9 register
#define IPR5n_PAL_TEMP10        0x14A   // (R/W) PALtemp 10 register
#define IPR5n_PAL_TEMP11        0x14B   // (R/W) PALtemp 11 register
#define IPR5n_PAL_TEMP12        0x14C   // (R/W) PALtemp 12 register
#define IPR5n_PAL_TEMP13        0x14D   // (R/W) PALtemp 13 register
#define IPR5n_PAL_TEMP14        0x14E   // (R/W) PALtemp 14 register
#define IPR5n_PAL_TEMP15        0x14F   // (R/W) PALtemp 15 register
#define IPR5n_PAL_TEMP16        0x150   // (R/W) PALtemp 16 register
#define IPR5n_PAL_TEMP17        0x151   // (R/W) PALtemp 17 register
#define IPR5n_PAL_TEMP18        0x152   // (R/W) PALtemp 18 register
#define IPR5n_PAL_TEMP19        0x153   // (R/W) PALtemp 19 register
#define IPR5n_PAL_TEMP20        0x154   // (R/W) PALtemp 20 register
#define IPR5n_PAL_TEMP21        0x155   // (R/W) PALtemp 21 register
#define IPR5n_PAL_TEMP22        0x156   // (R/W) PALtemp 22 register
#define IPR5n_PAL_TEMP23        0x157   // (R/W) PALtemp 23 register

// MTU IPR registers table
#define IPR5n_DTB_ASN           0x200   // (W)
#define IPR5n_DTB_CM            0x201   // (W)
#define IPR5n_DTB_TAG           0x202   // (W)
#define IPR5n_DTB_PTE           0x203   // (R/W)
#define IPR5n_DTB_PTE_TEMP      0x204   // (R)
#define IPR5n_MM_STAT           0x205   // (R)
#define IPR5n_VA                0x206   // (R)
#define IPR5n_VA_FORM           0x207   // (R)
#define IPR5n_MVPTBR            0x208   // (W)
#define IPR5n_DTB_IAP           0x209   // (W)
#define IPR5n_DTB_IA            0x20A   // (W)
#define IPR5n_DTB_IS            0x20B   // (W)
#define IPR5n_ALT_MODE          0x20C   // (W)
#define IPR5n_CC                0x20D   // (W)
#define IPR5n_CC_CTL            0x20E   // (W)
#define IPR5n_MCSR              0x20F   // (R/W)
#define IPR5n_DC_FLUSH          0x210   // (W)
#define IPR5n_DC_PERR_STAT      0x212`  // (R/W1C)
#define IPR5n_DC_TEST_CTL       0x213   // (R/W)
#define IPR5n_DC_TEST_TAG       0x214   // (R/W)
#define IPR5n_DC_TEST_TAG_TEMP  0x215   // (R/W)
#define IPR5n_DC_MODE           0x216   // (R/W)
#define IPR5n_MAF_MODE          0x217   // (R/W)