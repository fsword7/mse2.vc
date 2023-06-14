// generic.h - Address Space (Generic memory acceses) package
//
// Author:  Tim Stark
// Date:    Jun 14, 2023

#pragma once

namespace map
{

    constexpr offs_t convertAddressByte(offs_t addr, int aShift)
    {
        return aShift > 0 ? (addr << aShift) : (addr >> -aShift);
    }

    template <int dWidth, int aShift, endian_t eType, int tWidth, bool Aligned, typename T>
    HandlerSize_t<tWidth> readGeneric(T rop, offs_t addr, HandlerSize_t<tWidth> mask, CPUDevice *cpu)
    {
        using targetType = HandlerSize_t<tWidth>;
        using nativeType = HandlerSize_t<dWidth>;

        constexpr uint32_t targetBytes = 1u << tWidth;
        constexpr uint32_t targetBits = 8 * targetBytes;
        constexpr uint32_t nativeBytes = 1u << dWidth;
        constexpr uint32_t nativeBits = 8 * nativeBytes;
        constexpr uint32_t nativeMask = (dWidth - aShift >= 0) ? makeBitmask<uint32_t>(dWidth - aShift) : 0;
        constexpr uint32_t nativeStep = aShift >= 0 ? nativeBytes >> aShift : nativeBytes << -aShift;

        // Equal to native width and aligned, execute simple native reader
        if (nativeBytes == targetBytes && (Aligned || (addr & nativeMask) == 0))
            return rop(addr & ~nativeMask, mask, cpu);

        // If native bits is larger, see if we can do a single masked reader
        if (nativeBytes > targetBytes)
        {
            uint64_t offsBits = convertAddressByte(addr, aShift) & (nativeBytes - (Aligned ? targetBytes : 1));
            if (Aligned || (offsBits + targetBits <= nativeBits))
            {
                if (eType != LittleEndian)
                    offsBits = nativeBits - targetBits - offsBits;
                return rop(addr & ~nativeMask, (nativeType)mask << offsBits, cpu) >> offsBits;
            }
        }

        uint32_t offsBits = 8 * (convertAddressByte(addr, aShift) & (nativeBytes - 1));
        addr &= ~nativeMask;
    
        return 0;
    }

    template <int dWidth, int aShift, endian_t eType, int tWidth, bool Aligned, typename T>
    void writeGeneric(T wop, offs_t addr, HandlerSize_t<tWidth> data, HandlerSize_t<tWidth> mask, CPUDevice *cpu)
    {
        using nativeType = HandlerSize_t<dWidth>;

        constexpr uint32_t targetBytes = 1u << tWidth;
        constexpr uint32_t targetBits = 8 * targetBytes;
        constexpr uint32_t nativeBytes = 1u << dWidth;
        constexpr uint32_t nativeBits = 8 * nativeBytes;
        constexpr uint32_t nativeMask = (dWidth - aShift >= 0) ? makeBitmask<uint32_t>(dWidth - aShift) : 0;
        constexpr uint32_t nativeStep = aShift >= 0 ? nativeBytes >> aShift : nativeBytes << -aShift;

        // Equal to native width and aligned, execute simple native reader
        if (nativeBytes == targetBytes && (Aligned || (addr & nativeMask) == 0))
            return wop(addr & ~nativeMask, data, mask, cpu);

        // If native bits is larger, see if we can do a single masked reader
        if (nativeBytes > targetBytes)
        {
            uint64_t offsBits = convertAddressByte(addr, aShift) & (nativeBytes - (Aligned ? targetBytes : 1));
            if (Aligned || (offsBits + targetBits <= nativeBits))
            {
                if (eType != LittleEndian)
                    offsBits = nativeBits - targetBits - offsBits;
                return wop(addr & ~nativeMask, (nativeType)data << offsBits, (nativeType)mask << offsBits, cpu);
            }
        }

        uint32_t offsBits = 8 * (convertAddressByte(addr, aShift) & (nativeBytes - 1));
        addr &= ~nativeMask;

    }
}