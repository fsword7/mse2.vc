// maccess.h - memory access package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

#pragma once

namespace map
{
    template <int Level, int dWidth, int aShift, endian_t eType>
    class MemoryAccessSpecific
    {
        friend class AddressSpace;

        using uintx_t = typename HandlerSize<dWidth>::uintx_t;
        using nativeType = typename HandlerSize<dWidth>::uintx_t;

        // static constexpr int      pageBits   = determineDispatchLowBits(Level, dWidth, aShift);
        static constexpr uint64_t nativeBytes = 1ull << dWidth;
        static constexpr uint64_t nativeMask  = dWidth - aShift >= 0 ? (1ull << (dWidth - aShift)) - 1 : 0;

    public:
        MemoryAccessSpecific() = default;

    private:
        inline offs_t convertAddressToByte(offs_t addr, int addrShift)
        {
            return (addrShift >= 0) ? addr << addrShift : addr >> -addrShift;
        }

    public:
        uint8_t read8(offs_t addr, ProcessorDevice *cpu = nullptr)
        {
            return 0;
        }

        uint16_t read16(offs_t addr, ProcessorDevice *cpu = nullptr)
        {
            return 0;
        }

        uint16_t read16u(offs_t addr, ProcessorDevice *cpu = nullptr)
        {
            return 0;
        }

        uint16_t read32(offs_t addr, ProcessorDevice *cpu = nullptr)
        {
            return 0;
        }

        uint16_t read32u(offs_t addr, ProcessorDevice *cpu = nullptr)
        {
            return 0;
        }

        uint16_t read64(offs_t addr, ProcessorDevice *cpu = nullptr)
        {
            return 0;
        }

        uint16_t read64u(offs_t addr, ProcessorDevice *cpu = nullptr)
        {
            return 0;
        }


        void write8(offs_t addr, uint8_t data, ProcessorDevice *cpu = nullptr)
        {
        }

        void write16(offs_t addr, uint16_t data, ProcessorDevice *cpu = nullptr)
        {
        }

        void write16u(offs_t addr, uint16_t data, ProcessorDevice *cpu = nullptr)
        {
        }

        void write32(offs_t addr, uint32_t data, ProcessorDevice *cpu = nullptr)
        {
        }

        void write32u(offs_t addr, uint32_t data, ProcessorDevice *cpu = nullptr)
        {
        }

        void write64(offs_t addr, uint64_t data, ProcessorDevice *cpu = nullptr)
        {
        }

        void write64u(offs_t addr, uint64_t data, ProcessorDevice *cpu = nullptr)
        {
        }

    };

    template <int highBits, int dWidth, int aShift, endian_t eType>
    struct MemoryAccess
    {
        static constexpr int Level = determineDispatchLevel(highBits);

        using specific = MemoryAccessSpecific<Level, dWidth, aShift, eType>;
        // using cache = MemoryAccessCache<Level, dWidth, aShift, eType>;
    };
}