// heun.h - Handler Entry - Unmapped/NOP (No Operation)
//
// Author:  Tim Stark
// Date:    12/13/2021

#pragma once

namespace map
{
    template <int dWidth, int aShift>
    class HandlerReadUnmapped : public HandlerRead<dWidth, aShift>
    {
    public:
        using uintx_t = typename HandlerRead<dWidth, aShift>::uintx_t;
        using base = HandlerRead<dWidth, aShift>;

        HandlerReadUnmapped(AddressSpace *space)
        : HandlerRead<dWidth, aShift>(space, 0)
        { }

        ~HandlerReadUnmapped() = default;

        cstag_t getName() const { return "unmapped"; }

        uintx_t read(offs_t offset, uintx_t mask, ProcessorDevice *cpu) const override
        {
            return base::space->getUnmapped();
        }
    };

    template <int dWidth, int aShift>
    class HandlerWriteUnmapped : public HandlerWrite<dWidth, aShift>
    {
    public:
        using uintx_t = typename HandlerWrite<dWidth, aShift>::uintx_t;
        using base = HandlerWrite<dWidth, aShift>;

        HandlerWriteUnmapped(AddressSpace *space)
        : HandlerWrite<dWidth, aShift>(space, 0)
        { }

        ~HandlerWriteUnmapped() = default;

        cstag_t getName() const { return "unmapped"; }

        void write(offs_t offset, uintx_t data, uintx_t mask, ProcessorDevice *cpu) const override
        {
        }
    };

    // ********

    template <int dWidth, int aShift>
    class HandlerReadNop : public HandlerRead<dWidth, aShift>
    {
    public:
        using uintx_t = typename HandlerRead<dWidth, aShift>::uintx_t;
        using base = HandlerRead<dWidth, aShift>;

        HandlerReadNop(AddressSpace *space)
        : HandlerRead<dWidth, aShift>(space, 0)
        { }

        ~HandlerReadNop() = default;

        cstag_t getName() const { return "nop"; }

        uintx_t read(offs_t offset, uintx_t mask, ProcessorDevice *cpu) const override
        {
            return base::space->getUnmapped();
        }
    };

    template <int dWidth, int aShift>
    class HandlerWriteNop : public HandlerWrite<dWidth, aShift>
    {
    public:
        using uintx_t = typename HandlerWrite<dWidth, aShift>::uintx_t;
        using base = HandlerWrite<dWidth, aShift>;

        HandlerWriteNop(AddressSpace *space)
        : HandlerWrite<dWidth, aShift>(space, 0)
        { }

        ~HandlerWriteNop() = default;

        cstag_t getName() const { return "nop"; }

        void write(offs_t offset, uintx_t data, uintx_t mask, ProcessorDevice *cpu) const override
        {
        }
    };
}