// com8116.cpp - COM8116 Dual BAUD rate generator
//
// Author:  Tim Stark
// Date:    Jan 13, 2022

#include "emu/core.h"
#include "dev/chip/com8116.h"

com8116_Device::com8116_Device(const SystemConfig &config, const DeviceType &type,
    cstag_t &devName, Device *owner, uint64_t clock, const int *divisors)
: Device(config, type, devName, owner, clock), divisors(divisors)
{ }

// ********

DEFINE_DEVICE_TYPE(COM8116, com8116_Device, "COM8116", "COM8116 Dual BRG")
DEFINE_DEVICE_TYPE(COM5016_013, com5016_013_Device, "COM5016-013", "COM5016-013 Dual BRG")

com8116_Device::com8116_Device(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: com8116_Device(config, COM8116, devName, owner, clock, divisors_16x_5_688Mhz)
{ }

com5016_013_Device::com5016_013_Device(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: com8116_Device(config, COM5016_013, devName, owner, clock, divisors_16x_2_7648Mhz)
{ }

// ********

const int com8116_Device::divisors_16x_5_688Mhz[16] =
    { 6336, 4224, 2880, 2355, 2112, 1056, 528, 264, 176, 158, 132, 88, 66, 44, 33, 16 };

const int com8116_Device::divisors_16x_2_7648Mhz[16] =
    { 3456, 2304, 1571, 1285, 1152, 864, 576, 288, 144, 96, 86, 72, 48, 36, 18, 8 };

// ********

// set transmitting time rate
void com8116_Device::write8t(uint8_t data)
{
    uint64_t rate = getClock() / divisors[data & 0x0F];
}

// set receiving time rate
void com8116_Device::write8r(uint8_t data)
{
    uint64_t rate = getClock() / divisors[data & 0x0F];
}

void com8116_Device::write8tr(uint8_t data)
{
    write8t(data >> 4);
    write8r(data & 0x0F);
}

void com8116_Device::write8rt(uint8_t data)
{
    write8r(data >> 4);
    write8t(data & 0x0F);
}