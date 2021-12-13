// memlib.h - memory library package
//
// Author:  Tim Stark
// Date:    12/12/2021

#pragma once

namespace map
{
    class MemoryBlock
    {
    public:
        MemoryBlock(cstag_t &name, size_t bytes, int width, endian_t type)
        : name(name), length(bytes), width(width), eType(type)
        {
        }

    private:
        mutable std::vector<uint8_t> data;

        cstag_t     name;
        size_t      length;
        int         width;
        endian_t    eType;
    };

    class MemoryRegion
    {
    public:
        MemoryRegion(Machine *sys, cstag_t &name, size_t bytes, int width, endian_t type)
        : system(sys), name(name), width(width), eType(type)
        {
            assert(width == 8 || width == 16 || width == 32 || width == 64);

        }

    private:
        std::vector<uint8_t> data;

        Machine     *system = nullptr;
        cstag_t     name;
        int         width;
        endian_t    eType;
    };

    class MemoryShare
    {
    public:
        MemoryShare(void *data, cstag_t &name, uint8_t width, size_t bytes, endian_t type)
        : data(data), size(bytes), name(name), eType(type), bitWidth(width),
          byteWidth(width <= 8 ? 1 : width <= 16 ? 2 : width <= 32 ? 4 : 8)
        {
        }

    private:
        void        *data;
        size_t      size;
        cstag_t     name;
        endian_t    eType;
        int         bitWidth;
        int         byteWidth;
    };

    class MemoryBank
    {

    };

    using BlockList  = std::vector<MemoryBlock *>;
    using RegionList = std::map<std::string, MemoryRegion *>;
    using ShareList  = std::map<std::string, MemoryShare *>;
    using BankList   = std::map<std::string, MemoryBank *>;
}