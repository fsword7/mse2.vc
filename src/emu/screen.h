// screen.h - screen device package
//
// Author:  Tim Stark
// Date:    12/23/21

#pragma once

#include "lib/util/bitmap.h"

enum ScreenType
{
    scrInvalid = 0,
    scrRaster,
    scrVector,
    scrPrinter
};

enum ScreenFormat
{
    scrUndefined = 0,
    scrPalette16,
    scrRGB32,
    scrRGBA32
};

constexpr uint64_t SCRFLAG_LANDSCAPE = 0;
constexpr uint64_t SCRFLAG_PORTRAIT = 0;

class ScreenDevice;

typedef DeviceDelegate<uint32_t (ScreenDevice &, bitmap16_t &, const rect_t &)> screenUpdateDelegate16;
typedef DeviceDelegate<uint32_t (ScreenDevice &, bitmap32_t &, const rect_t &)> screenUpdateDelegate32;

class ScreenDevice : public Device
{
public:
    ScreenDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);

    ScreenDevice(const SystemConfig &config, cstag_t &devName, Device *owner, ScreenType type)
    : ScreenDevice(config, devName, owner, 0)
    {
        setScreenType(type);
    }

    ScreenDevice(const SystemConfig &config, cstag_t &devName, Device *owner, ScreenType type, Color color)
    : ScreenDevice(config, devName, owner, 0)
    {
        setScreenType(type);
        setColor(color);
    }

    ~ScreenDevice() = default;

    inline void setColor(Color newColor)        { color = newColor; }
    inline void setScreenType(ScreenType sType) { type = sType; }

    void setScreenArea(uint64_t pixClock,
        uint16_t htotal, uint16_t hstart, uint16_t hend,
        uint16_t vtotal, uint16_t vstart, uint16_t vend)
    {
        assert(pixClock != 0);

        // Set visual coordinates with video blank edges
        width = htotal;
        height = vtotal;
        visualArea.set(hstart, hend ? hend-1 : htotal-1, vstart, vend-1);

        // Calculate timing settings
    }

    template <typename F> std::enable_if_t<screenUpdateDelegate16::supportCallback<F>::value>
    setScreenUpdate(F &&callback, ctag_t *fncName)
    {
        updateScreen16.set(std::forward<F>(callback), fncName);
        updateScreen32 = screenUpdateDelegate32(*this);
    }

    template <typename F> std::enable_if_t<screenUpdateDelegate32::supportCallback<F>::value>
    setScreenUpdate(F &&callback, ctag_t *fncName)
    {
        updateScreen16 = screenUpdateDelegate16(*this);
        updateScreen32.set(std::forward<F>(callback), fncName);
    }

private:
    const SystemDriver &driver;

    ScreenType type = scrInvalid;
    ScreenFormat format = scrUndefined;
    uint64_t flags = 0;

    // Screen parameter definitions
    int width = 100;
    int height = 100;
    rect_t visualArea = { 0, width-1, 0, height-1 };

    uint8_t brightness = 0xFF;
    Color color = Color::white();

    screenUpdateDelegate16 updateScreen16;
    screenUpdateDelegate32 updateScreen32;
};

DECLARE_DEVICE_TYPE(Screen, ScreenDevice);

using screen_t = ScreenDevice;