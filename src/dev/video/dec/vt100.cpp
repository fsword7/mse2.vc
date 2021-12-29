// vt100.cpp - DEC VT100 terminal - DC011/DC012 video controller
//
// Author:  Tim Stark
// Date:    12/15/2021

#include "emu/core.h"
#include "dev/video/dec/vt100.h"

using namespace emu::video;

DEFINE_DEVICE_TYPE(VT100_VIDEO, vt100_videoDevice, "VT100_Video", "VT100 video controller");

vt100_videoDevice::vt100_videoDevice(const SystemConfig &config, const DeviceType &type,
    cstag_t &devName, Device *owner, uint64_t clock)
: Device(config, type, devName, owner, clock),
  diVideo(this)
{
}

vt100_videoDevice::vt100_videoDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: vt100_videoDevice(config, VT100_VIDEO, devName, owner, clock)
{
}

void vt100_videoDevice::updateVideo(bitmap16_t &bitmap, const rect_t &clip)
{

}

uint8_t vt100_videoDevice::read8_test(offs_t offset)
{
    return offset;
}

void vt100_videoDevice::write8_dc011(offs_t offset, uint8_t data)
{
}

void vt100_videoDevice::write8_dc012(offs_t offset, uint8_t data)
{
}

void vt100_videoDevice::write8_brightness(offs_t offset, uint8_t data)
{
}