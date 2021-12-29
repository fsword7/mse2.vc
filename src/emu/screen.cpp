// screen.cpp - screen device package
//
// Author:  Tim Stark
// Date:    12/23/21

#include "emu/core.h"
#include "emu/screen.h"

DEFINE_DEVICE_TYPE(Screen, ScreenDevice, "Screen", "Video screen");

ScreenDevice::ScreenDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: Device(config, Screen, devName, owner, clock),
  driver(config.getSystemDriver()),
  updateScreen16(*this),
  updateScreen32(*this)
{

}