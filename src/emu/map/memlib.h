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
        : name(name), size(bytes), width(width), eType(type)
        {
            assert(width == 8 || width == 16 || width == 32 || width == 64);
            
            // Now allocating memory space
            data.resize(bytes);
        }

        inline uint8_t *getData() const { return data.data(); }

    private:
        mutable std::vector<uint8_t> data;

        cstag_t     name;
        size_t      size;
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

            data.resize(bytes);
        }

        inline uint8_t *getData() const { return data.data(); }
        inline size_t getSize() const   { return data.size(); }
        inline size_t getBytes() const  { return data.size(); }

        uint8_t *getBase() { return data.size() > 0 ? data.data() : nullptr; }
        uint8_t *getEnd()  { return getBase() + data.size(); }

    private:
        mutable std::vector<uint8_t> data;

        Machine     *system = nullptr;
        size_t      size;
        cstag_t     name;
        int         width;
        endian_t    eType;
    };

    class MemoryShare
    {
    public:
        MemoryShare(void *data, cstag_t &name, size_t bytes, uint8_t width, endian_t type)
        : data(data), size(bytes), name(name), eType(type), bitWidth(width),
          byteWidth(width <= 8 ? 1 : width <= 16 ? 2 : width <= 32 ? 4 : 8)
        {
        }

        inline cstag_t  getName() const         { return name; }
        inline uint8_t *getData() const         { return reinterpret_cast<uint8_t *>(data); }
        inline size_t   getBytes() const        { return size; }
        inline int      getBitWidth() const     { return bitWidth; }
        inline int      getByteWidth() const    { return byteWidth; }
        inline cstag_t &getErrorMessage() const { return errMessage; }

        bool compare(size_t bytes, int precision, int width, endian_t type)
        {
            if (width != bitWidth)
            {
                errMessage = fmt::sprintf("share '%s' found with unexpected width (expected %d, found %d)",
                    name, width, bitWidth);
                return false;
            }

            if (bytes != size)
            {
                errMessage = fmt::sprintf("share '%s' found with unexpected size (expected %0*llX, found %0*llX)",
                    name, precision, bytes, precision, size);
                return false;
            }

            if (type != eType)
            {
                errMessage = fmt::sprintf("share '%s' found with unexpected endian type (expected %s, found %s)",
                    name, type  == LittleEndian ? "little" : "big",
                    eType == LittleEndian ? "little" : "big");
                return false;
            }

            return true;
        }

    private:
        void        *data;
        size_t      size;
        cstag_t     name;
        endian_t    eType;
        int         bitWidth;
        int         byteWidth;

        std::string errMessage;
    };

    class MemoryBank
    {
    public:
        MemoryBank(cstag_t tagName)
        {
            name = fmt::sprintf("Bank '%s'", tagName);

            entries.clear();
        }
        ~MemoryBank() = default;

        inline uint8_t *getBase() const { return entries[idxEntry]; }
        inline cstag_t getName() const  { return name; }

        void configureEntries(int startEntry, int nEntries, void *base, offs_t stride)
        {
            if (startEntry + nEntries >= entries.size())
                entries.resize(startEntry + nEntries + 1);
            for (int idx = 0; idx < nEntries; idx++)
                entries[startEntry + idx] = reinterpret_cast<uint8_t *>(base) + (stride * idx);
        }

        void configureEntry(int entry, void *base)
        {
            if (entry < 0)
                return;
            if (entry >= entries.size())
                entries.resize(entry+1);
            entries[entry] = reinterpret_cast<uint8_t *>(base);
        }

        void setEntry(int entry)
        {
            if (entry < 0 && entries.empty())
                return;
            if (entry < 0 || entry >= entries.size())
                return;
            if (entries[entry] == nullptr)
                return;
            idxEntry = entry;
        }

    private:
        std::vector<uint8_t *> entries;
        int idxEntry = 0;
        std::string name;
    };

    using BlockList  = std::vector<MemoryBlock *>;
    using RegionList = std::map<std::string, MemoryRegion *>;
    using ShareList  = std::map<std::string, MemoryShare *>;
    using BankList   = std::map<std::string, MemoryBank *>;
}