// vt100.h - DEC VT100 terminal - DC011/DC012 video controller
//
// Author:  Tim Stark
// Date:    12/15/2021

#pragma once

#include "emu/divideo.h"

namespace emu::video
{
    class vt100_videoDevice : public Device, public diVideo
    {
    public:
        vt100_videoDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);

        void updateVideo(bitmap16_t &bitmap, const rect_t &clip);
        
        // I/O acccess function calls
        void write8_dc011(offs_t offset, uint8_t data);
        void write8_dc012(offs_t offset, uint8_t data);
        void write8_brightness(offs_t offset, uint8_t data);

        uint8_t read8_test(offs_t offset);
        
    protected:
        vt100_videoDevice(const SystemConfig &config, const DeviceType &type,
            cstag_t &devName, Device *owner, uint64_t clock);

    };
}

DECLARE_DEVICE_TYPE(VT100_VIDEO, emu::video::vt100_videoDevice);

using vt100video_t = emu::video::vt100_videoDevice;
