// rs232.cpp - RS232 communication package
//
// Author:  Tim Stark
// Date:    Jan 12, 2022

#include "emu/core.h"
#include "emu/devcb.h"
#include "dev/bus/rs232/rs232.h"

rs232_portDevice::rs232_portDevice(const SystemConfig &config, const DeviceType &type,
    cstag_t &devName, Device *owner, uint64_t clock)
: Device(config, type, devName, owner, clock),
  rxdHandler(*this), dcdHandler(*this), dsrHandler(*this),
  riHandler(*this),  siHandler(*this),  ctsHandler(*this),
  rxcHandler(*this), txcHandler(*this)
{ }

DEFINE_DEVICE_TYPE(RS232_PORT, rs232_portDevice, "RS232", "RS-232 communication port");

rs232_portDevice::rs232_portDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: rs232_portDevice(config, RS232_PORT, devName, owner, clock)
{ }