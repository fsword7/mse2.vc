// bitmap.h - bitmap package
//
// Author:  Tim Stark
// Date:    12/24/21

#pragma once

namespace emu::graphics
{
    enum BitmapFormat
    {
        BitmapFormatInvalid = 0,
        BitmapFormatIndex8,
        BitmapFormatIndex16,
        BitmapFormatIndex32,
        BitmapFormatIndex64,
        BitmapFormatRGB32,
        BitmapFormatRGBA32
    };

    class Rectangle
    {
    public:
        constexpr Rectangle() = default;
        constexpr Rectangle(int xmin, int xmax, int ymin, int ymax)
        : yMin(ymin), yMax(ymax), xMin(xmin), xMax(xmax)
        { }

        constexpr void set(int xmin, int xman, int ymin, int ymax)
        { xMin = xmin; xMax = xman; yMin = ymin; yMax = ymax; }

    private:
        int xMin = 0; // left coordinate
        int xMax = 0; // right coordinate
        int yMin = 0; // top coordinate
        int yMax = 0; // bottom coordinate
    };

    class Bitmap
    {
    public:
        Bitmap() = default;
    
    private:

    };

    template <typename PixelType>
    class BitmapSpecific : public Bitmap
    {
        static constexpr int PIXEL_BITS = sizeof(PixelType) * 8;

    public:
        using pixel_t = PixelType;
    };
}

using rect_t     = emu::graphics::Rectangle;

using bitmap_t   = emu::graphics::Bitmap;
using bitmap8_t  = emu::graphics::BitmapSpecific<uint8_t>;
using bitmap16_t = emu::graphics::BitmapSpecific<uint16_t>;
using bitmap32_t = emu::graphics::BitmapSpecific<uint32_t>;
using bitmap64_t = emu::graphics::BitmapSpecific<uint64_t>;
