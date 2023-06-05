// ev4cpu.h - Alpha EV4 Procssor package (21064 series)
//
// Author:  Tim Stark
// Date:    May 21, 2023

#pragma once

// 21064/21064A HRM Section 4.8.1 (page 4-20 - 4-21)
// MTPR/MFPR instruction defintions
#define MxPR_PAL    0x80    // MTPR/MFPR bit 7
#define MxPR_ABX    0x40    // MTPR/MFPR bit 6
#define MxPR_IBX    0x20    // MTPR/MFPR bit 5
#define MxPR_INDEX  0x1F    // MYPR/MFPR index regiser

class EV4Processor : public AlphaProcessor
{
public:
    EV4Processor(SystemConfig &config, cDeviceType &type,
        cstr_t &devName, Device *owner, uint64_t clock);
    ~EV4Processor() = default;

    void pal_call();
    void hw_ld();
    void hw_st();
    void hw_rei();
    void hw_mtpr();
    void hw_mfpr();

private:
    // Interal procesor registers
    struct ev4state
    {
        // IBOX registers (20-3F)

        // ABOX registers (40-5F)

        // PAL registers (80-9F)
        uint64_t palTemp[32];   // PALtemp registers

    } ev4;
};

class axp21064_cpuDevice : public EV4Processor
{
public:
	axp21064_cpuDevice(SystemConfig &config, cstr_t &devName, Device *owner, uint64_t clock);
};

DECLARE_DEVICE_TYPE(AXP21064, axp21064_cpuDevice);

