// i8155.cpp - Intel 8155/8156 2048-bit static MOS RAM with I/O ports and timer
//
// Author:  Tim Stark
// Date:    Jan 4, 2022

#include "emu/core.h"
#include "dev/chip/i8155.h"


i8155_Device::i8155_Device(const SystemConfig &config, const DeviceType &type, cstag_t &devName, Device *owner, uint64_t clock)
: Device(config, type, devName, owner, clock)
{ }

// ********

DEFINE_DEVICE_TYPE(I8155, i8155_Device, "i8155", "Intel 8155 RAM, I/O & Timer")
DEFINE_DEVICE_TYPE(I8156, i8156_Device, "i8156", "Intel 8155 RAM, I/O & Timer")

i8155_Device::i8155_Device(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: i8155_Device(config, I8155, devName, owner, clock)
{ }

i8156_Device::i8156_Device(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: i8155_Device(config, I8156, devName, owner, clock)
{ }

// ********

