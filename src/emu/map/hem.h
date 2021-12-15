// hem.h - Handler Entry - Memory access
// 
// Author:  Tim Stark
// Date:    12/14/2021

#pragma once

namespace map
{
    template <int dWidth, int aShift>
    class HandlerReadMemory : public HandlerReadAddress<dWidth, aShift>
    {
    public:
        using uintx_t = typename HandlerSize<dWidth>::uintx_t;
 
        HandlerReadMemory(AddressSpace *space, void *base)
        : HandlerReadAddress<dWidth, aShift>(space, 0),
          base(reinterpret_cast<uintx_t *>(base))
        { 
            assert(base != nullptr);
        }

        ~HandlerReadMemory() = default;

        cstag_t getName() const { return "memory"; }

        uintx_t read(offs_t offset, uintx_t mask, ProcessorDevice *cpu) const override
        {
            return base[((offset - this->baseAddress) & this->maskAddress) >> std::max(0, dWidth - aShift)];
        }

        void *getAccess(offs_t offset) const override
        {
            return &base[((offset - this->baseAddress) & this->maskAddress) >> std::max(0, dWidth - aShift)];
        }

    private:
        mutable uintx_t *base = nullptr;
    };

    template <int dWidth, int aShift>
    class HandlerWriteMemory : public HandlerWriteAddress<dWidth, aShift>
    {
    public:
        using uintx_t = typename HandlerSize<dWidth>::uintx_t;
 
        HandlerWriteMemory(AddressSpace *space, void *base)
        : HandlerWriteAddress<dWidth, aShift>(space, 0),
          base(reinterpret_cast<uintx_t *>(base))
        {
            assert(base != nullptr);
        }

        ~HandlerWriteMemory() = default;

        cstag_t getName() const { return "memory"; }

        void write(offs_t offset, uintx_t data, uintx_t mask, ProcessorDevice *cpu) const override
        {
            offs_t off = ((offset - this->baseAddress) & this->maskAddress) >> std::max(0, dWidth - aShift);
            base[off] = (base[off] & ~mask) | (data & mask);
        }

        void *getAccess(offs_t offset) const override
        {
            return &base[((offset - this->baseAddress) & this->maskAddress) >> std::max(0, dWidth - aShift)];
        }

    private:
        mutable uintx_t *base = nullptr;
    }; 
}