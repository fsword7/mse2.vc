// maccess.h - Memory Access package
//
// Author:  Tim Stark
// Date:    May 14, 2023

#pragma once

namespace map
{
    template <int Level,  int dWidth, int aShift, endian_t eType>
    class MemoryAccessSpecific
    {
        using uintx_t = HandlerSize_t<dWidth>;
        using nativeType = HandlerSize_t<dWidth>;

        static constexpr int      pageBits    = std::min(0, determineDispatchLowBits(Level, dWidth, aShift));
        static constexpr uint64_t nativeBytes = 1ull << dWidth;
        static constexpr uint64_t nativeMask  = dWidth - aShift >= 0 ? (1ull << (dWidth - aShift)) - 1 : 0;

        // AddressSpace *space = nullptr;
        offs_t addrMask = 0;
        uint64_t unmapValue = 0;

        offs_t memSize = 0;
        uint8_t *memData = nullptr;

    public:
        MemoryAccessSpecific() = default;

    private:
        inline offs_t convertAddressToByte(offs_t addr, int addrShift)
        {
            return (addrShift >= 0) ? addr << addrShift : addr >> -addrShift;
        }

        // void set(AddressSpace *space, offs_t mask, const void *read, const void *write)
        // {
        //     assert (read != nullptr && write != nullptr);
            
        //     this->space = space;
        //     addrMask = mask;
        //     readDispatch = (const HandlerRead<dWidth, aShift> *const *)(read);
        //     writeDispatch = (const HandlerWrite<dWidth, aShift> *const *)(write);
        // }

        inline uintx_t readNative(offs_t addr, uintx_t mask, CPUDevice *cpu)
        {
            // return readDispatch[(addr & addrMask) >> pageBits]->read(addr, mask, cpu);
            return 0;
        }

        inline void writeNative(offs_t addr, uintx_t data, uintx_t mask, CPUDevice *cpu)
        {
            // writeDispatch[(addr & addrMask) >> pageBits]->write(addr, data, mask, cpu);
        }

    public:
        uint8_t read8(offs_t addr, CPUDevice *cpu = nullptr)
        {
            if (dWidth == 0)
                return readNative(addr, 0xFF, cpu);
            return 0;
        }

        uint16_t read16(offs_t addr, CPUDevice *cpu = nullptr)
        {
            if (dWidth == 1)
                return readNative(addr, 0xFFFF, cpu);
            return 0;
        }

        uint16_t read16u(offs_t addr, CPUDevice *cpu = nullptr)
        {   
            return 0;
        }

        uint16_t read32(offs_t addr, CPUDevice *cpu = nullptr)
        {   
            if (dWidth == 2)
                return readNative(addr, 0xFFFF'FFFF, cpu);
            return 0;
        }

        uint16_t read32u(offs_t addr, CPUDevice *cpu = nullptr)
        {   
            return 0;
        }

        uint16_t read64(offs_t addr, CPUDevice *cpu = nullptr)
        {   
            if (dWidth == 3)
                return readNative(addr, 0xFFFF'FFFF'FFFF'FFFFull, cpu);
            return 0;
        }

        uint16_t read64u(offs_t addr, CPUDevice *cpu = nullptr)
        {   
            return 0;
        }


        void write8(offs_t addr, uint8_t data, CPUDevice *cpu = nullptr)
        {
            if (dWidth == 0)
                writeNative(addr, data, 0xFF, cpu);
        }

        void write16(offs_t addr, uint16_t data, CPUDevice *cpu = nullptr)
        {
            if (dWidth == 1)
                writeNative(addr, data, 0xFFFF, cpu);
        }

        void write16u(offs_t addr, uint16_t data, CPUDevice *cpu = nullptr)
        {
        }

        void write32(offs_t addr, uint32_t data, CPUDevice *cpu = nullptr)
        {
            if (dWidth == 2)
                writeNative(addr, data, 0xFFFF'FFFF, cpu);
        }

        void write32u(offs_t addr, uint32_t data, CPUDevice *cpu = nullptr)
        {
        }

        void write64(offs_t addr, uint64_t data, CPUDevice *cpu = nullptr)
        {
            if (dWidth == 3)
                writeNative(addr, data, 0xFFFF'FFFF'FFFF'FFFFull, cpu);
        }

        void write64u(offs_t addr, uint64_t data, CPUDevice *cpu = nullptr)
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