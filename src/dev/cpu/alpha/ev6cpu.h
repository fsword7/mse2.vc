// ev6cpu.h - Alpha EV6 Procssor package (21264 series)
//
// Author:  Tim Stark
// Date:    May 21, 2023

#pragma once

class EV6Processor : public AlphaProcessor
{
public:
    EV6Processor(SystemConfig &config, cDeviceType &type,
        cstr_t &devName, Device *owner, uint64_t clock);
    ~EV6Processor() = default;
    
    void pal_call();
    void hw_ld();
    void hw_st();
    void hw_rei();
    void hw_mtpr();
    void hw_mfpr();

private:
    // Interal procesor registers
    struct ev6state
    {

        uint64_t palTemp[24];   // PALtemp registers
    } ev6;
};

class axp21264_cpuDevice : public EV6Processor
{
public:
	axp21264_cpuDevice(SystemConfig &config, cstr_t &devName, Device *owner, uint64_t clock);
};

DECLARE_DEVICE_TYPE(AXP21264, axp21264_cpuDevice);

