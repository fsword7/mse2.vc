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

        cstag_t getName() const { return fmt::sprintf("memory @%llX", this->baseAddress); }

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

        cstag_t getName() const { return fmt::sprintf("memory @%llX", this->baseAddress); }

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

    template <int dWidth, int aShift>
    class HandlerReadMemoryBank : public HandlerReadAddress<dWidth, aShift>
    {
    public:
        using uintx_t = typename HandlerSize<dWidth>::uintx_t;
 
        HandlerReadMemoryBank(AddressSpace *space, uint32_t flags, MemoryBank &bank)
        : HandlerReadAddress<dWidth, aShift>(space, flags), bank(bank)
        { }

        ~HandlerReadMemoryBank() = default;

        cstag_t getName() const { return bank.getName(); }

        uintx_t read(offs_t offset, uintx_t mask, ProcessorDevice *cpu) const override
        {
            return bank.getBase()[((offset - this->baseAddress) & this->maskAddress) >> std::max(0, dWidth - aShift)];
        }

        void *getAccess(offs_t offset) const override
        {
            return &bank.getBase()[((offset - this->baseAddress) & this->maskAddress) >> std::max(0, dWidth - aShift)];
        }

    private:
        MemoryBank &bank;
    };

    template <int dWidth, int aShift>
    class HandlerWriteMemoryBank : public HandlerWriteAddress<dWidth, aShift>
    {
    public:
        using uintx_t = typename HandlerSize<dWidth>::uintx_t;
 
        HandlerWriteMemoryBank(AddressSpace *space, uint32_t flags, MemoryBank &bank)
        : HandlerWriteAddress<dWidth, aShift>(space, flags), bank(bank)
        { }

        ~HandlerWriteMemoryBank() = default;

        cstag_t getName() const { return bank.getName(); }

        void write(offs_t offset, uintx_t data, uintx_t mask, ProcessorDevice *cpu) const override
        {
            offs_t off = ((offset - this->baseAddress) & this->maskAddress) >> std::max(0, dWidth - aShift);
            bank.getBase()[off] = (bank.getBase()[off] & ~mask) | (data & mask);
        }

        void *getAccess(offs_t offset) const override
        {
            return &bank.getBase()[((offset - this->baseAddress) & this->maskAddress) >> std::max(0, dWidth - aShift)];
        }

    private:
        MemoryBank &bank;
    };
 
}