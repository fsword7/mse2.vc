// i8251.cpp - Intel 8251 USART (Universal Synchronous/Asynchronous Receiver Transmitter)
//
// Author:  Tim Stark
// Date:    Jan 12, 2022

#include "emu/core.h"
#include "dev/chip/i8251.h"

i8251_Device::i8251_Device(const SystemConfig &config, const DeviceType &type, cstag_t &devName, Device *owner, uint64_t clock)
: Device(config, type, devName, owner, clock), diSerial(*this)
{ }

// ********

DEFINE_DEVICE_TYPE(I8251, i8251_Device, "i8251", "Intel 8251 USART")

i8251_Device::i8251_Device(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: i8251_Device(config, I8251, devName, owner, clock)
{ }
