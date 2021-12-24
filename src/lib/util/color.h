// color.h - color/palette package
//
// Author:  Tim Stark
// Date:    12/24/21

#pragma once

// SDR ARGB8 color
class Color
{
public:
    constexpr Color() = default;
    constexpr Color(uint32_t argb) : data(argb) {}

    constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
    : data((a << 24) | (r << 16) | (g << 8) | b)
    { }

    // Getter function calls
    constexpr uint8_t getAlpha() { return data >> 24; }
    constexpr uint8_t getRed()   { return data >> 16; }
    constexpr uint8_t getGreen() { return data >> 8; }
    constexpr uint8_t getBlue()  { return data; }

    // Colors for monochrome monitors
    static constexpr Color black() { return Color(0, 0, 0); }
    static constexpr Color white() { return Color(255, 255, 255); }
    static constexpr Color green() { return Color(0, 255, 0); }
    static constexpr Color amber() { return Color(255, 191, 0); }
 
    static constexpr Color transparent() { return Color(0, 0, 0, 0); }

private:
    uint32_t data = 0;
};