// com8116.h - COM8116 Dual BAUD rate generator
//
// Author:  Tim Stark
// Date:    Jan 13, 2022

#pragma once

class com8116_Device : public Device
{
public:
    com8116_Device(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);

    static const int divisors_16x_5_688Mhz[16];
    static const int divisors_16x_2_7648Mhz[16];

    // function calls
    void write8t(uint8_t data);
    void write8r(uint8_t data);
    void write8tr(uint8_t data);
    void write8rt(uint8_t data);

protected:
    com8116_Device(const SystemConfig &config, const DeviceType &type,
        cstag_t &devName, Device *owner, uint64_t clock, const int *divisors);

private:

    const int *const divisors;
};

class com5016_013_Device : public com8116_Device
{
public:
    com5016_013_Device(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);

};

DECLARE_DEVICE_TYPE(COM8116, com8116_Device)
DECLARE_DEVICE_TYPE(COM5016_013, com5016_013_Device)