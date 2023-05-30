// memlib.h - Memory Library package
//
// Author:  Tim Stark
// Date:    May 11, 2023

namespace map
{
    class MemoryBlock
    {
    public:
        MemoryBlock(cstr_t &name, size_t bytes, int width, endian_t type)
        : name(name), size(bytes), width(width), eType(type)
        {
            assert(width == 8 || width == 16 || width == 32 || width == 64);
            data.resize(bytes);
        }

        inline uint8_t *getData() const { return data.data(); }
        inline size_t getSize() const   { return data.size(); }
        inline size_t getBytes() const  { return data.size(); }

    private:
        mutable std::vector<uint8_t> data;

        cstr_t   &name;
        size_t    size;
        int       width;
        endian_t  eType;
    };

    class MemoryRegion
    {
    public:
        MemoryRegion(Machine *sys, cstr_t &name, size_t bytes, int width, endian_t type)
        : sysMachine(sys), name(name), size(bytes), width(width), eType(type)
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

        Machine  *sysMachine = nullptr;
        cstr_t   &name;
        size_t    size;
        int       width;
        endian_t  eType;
    };

    class MemoryShare
    {

    };
    class MemoryBank
    {

    };

    using BlockList  = std::vector<MemoryBlock *>;
    using RegionList = std::map<str_t, MemoryRegion *>;
    using ShareList  = std::map<str_t, MemoryShare *>;
    using BankList   = std::map<str_t, MemoryBank *>;
}