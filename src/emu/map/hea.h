// hea.h - Handler Entry - Address Space
//
// Author:  Tim Stark
// Date:    12/14/2021

#pragma once

namespace map
{
    template <dWidth, aShift>
    class HandlerReadAddress : public HandlerRead<dWidth, aShift>
    {
    public:
        using uintx_t = typename HandlerSize<dWidth>::uintx_t;

        HandlerReadAddress(AddressSpace *space, uint32_t flags)
        : HandlerRead<dWidth, aShift>(space, flags)
        { 
        }

        ~HandlerReadAddress() = default;

        inline void setAddressSpace(offs_t base, offs_t mask)
        {
            baseAddress = base & ~HandlerRead<dWidth, aShift>::nativeMask;
            maskAddress = mask;
        }

    protected:
        offs_t baseAddress = 0;
        offs_t maskAddress = 0;
    };

    template <dWidth, aShift>
    class HandlerWriteAddress : public HandlerWrite<dWidth, aShift>
    {
    public:
        using uintx_t = typename HandlerSize<dWidth>::uintx_t;

        HandlerWriteAddress(AddressSpace *space, uint32_t flags)
        : HandlerWrite<dWidth, aShift>(space, flags)
        { }

        ~HandlerWriteAddress() = default;

        inline void setAddressSpace(offs_t base, offs_t mask)
        {
            baseAddress = base & ~HandlerRead<dWidth, aShift>::nativeMask;
            maskAddress = mask;
        }

    protected:
        offs_t baseAddress = 0;
        offs_t maskAddress = 0;
    };
}