// heun.h - Handler Entry - Unmapped/NOP (No Operation)
//
// Author:  Tim Stark
// Date:    6/5/2023

#pragma once

namespace map
{
    template <int dWidth, int aShift>
    class HandlerReadUnmapped : public HandlerRead<dWidth, aShift>
    {
    public:
        using uintx_t = typename HandlerRead<dWidth, aShift>::uintx_t;
        using base = HandlerRead<dWidth, aShift>;

        HandlerReadUnmapped(AddressSpace *space, uint32_t flags = 0)
        : HandlerRead<dWidth, aShift>(space, flags)
        { }

        ~HandlerReadUnmapped() = default;

        cstr_t getName() const { return "unmapped"; }

        uintx_t read(offs_t offset, uintx_t mask, CPUDevice *cpu) const override
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

        HandlerWriteUnmapped(AddressSpace *space, uint32_t flags = 0)
        : HandlerWrite<dWidth, aShift>(space, flags)
        { }

        ~HandlerWriteUnmapped() = default;

        cstr_t getName() const { return "unmapped"; }

        void write(offs_t offset, uintx_t data, uintx_t mask, CPUDevice *cpu) const override
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

        HandlerReadNop(AddressSpace *space, uint32_t flags = 0)
        : HandlerRead<dWidth, aShift>(space, flags)
        { }

        ~HandlerReadNop() = default;

        cstr_t getName() const { return "nop"; }

        uintx_t read(offs_t offset, uintx_t mask, CPUDevice *cpu) const override
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

        HandlerWriteNop(AddressSpace *space, uint32_t flags = 0)
        : HandlerWrite<dWidth, aShift>(space, flags)
        { }

        ~HandlerWriteNop() = default;

        cstr_t getName() const { return "nop"; }

        void write(offs_t offset, uintx_t data, uintx_t mask, CPUDevice *cpu) const override
        {
        }
    };
}