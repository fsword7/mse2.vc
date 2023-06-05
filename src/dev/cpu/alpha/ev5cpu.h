// ev5cpu.h - Alpha EV5 Procssor package (21164 series)
//
// Author:  Tim Stark
// Date:    May 21, 2023

#pragma once

class EV5Processor : public AlphaProcessor
{
public:
    EV5Processor(SystemConfig &config, cDeviceType &type,
        cstr_t &devName, Device *owner, uint64_t clock);
    ~EV5Processor() = default;

    void pal_call();
    void hw_ld();
    void hw_st();
    void hw_rei();
    void hw_mtpr();
    void hw_mfpr();

private:
    // Interal procesor registers
    struct ev5state
    {

        uint64_t palTemp[24];   // PALtemp registers
    } ev5;
};
class axp21164_cpuDevice : public EV5Processor
{
public:
	axp21164_cpuDevice(SystemConfig &config, cstr_t &devName, Device *owner, uint64_t clock);
};

DECLARE_DEVICE_TYPE(AXP21164, axp21164_cpuDevice);


