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
    public:
        MemoryShare(void *data, cstr_t &name, size_t bytes, uint8_t width, endian_t type)
        : data(data), size(bytes), name(name), eType(type), bitWidth(width),
          byteWidth(width <= 8 ? 1 : width <= 16 ? 2 : width <= 32 ? 4 : 8)
        {
        }

        inline cstr_t   getName() const         { return name; }
        inline uint8_t *getData() const         { return reinterpret_cast<uint8_t *>(data); }
        inline size_t   getBytes() const        { return size; }
        inline int      getBitWidth() const     { return bitWidth; }
        inline int      getByteWidth() const    { return byteWidth; }
        inline cstr_t  &getErrorMessage() const { return errMessage; }

        bool compare(size_t bytes, int precision, int width, endian_t type)
        {
            if (width != bitWidth)
            {
                errMessage = fmt::format("share '{}' found with unexpected width (expected {}, found {})",
                    name, width, bitWidth);
                return false;
            }

            if (bytes != size)
            {
                errMessage = fmt::format("share '{}' found with unexpected size (expected {:0{}X}, found {:0{}X})",
                    name, precision, bytes, precision, size);
                return false;
            }

            if (type != eType)
            {
                errMessage = fmt::format("share '{}' found with unexpected endian type (expected {}, found {})",
                    name, type  == LittleEndian ? "little" : "big",
                    eType == LittleEndian ? "little" : "big");
                return false;
            }

            return true;
        }

    private:
        void       *data;
        size_t      size;
        cstr_t      name;
        endian_t    eType;
        int         bitWidth;
        int         byteWidth;

        str_t       errMessage;
    };
    class MemoryBank
    {
    public:
        MemoryBank(cstr_t tagName)
        {
            name = fmt::format("Bank '{}'", tagName);

            entries.clear();
        }
        ~MemoryBank() = default;

        inline uint8_t *getBase() const { return entries[idxEntry]; }
        inline cstr_t getName() const   { return name; }

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
        str_t name;
    };


    using BlockList  = std::vector<MemoryBlock *>;
    using RegionList = std::map<str_t, MemoryRegion *>;
    using ShareList  = std::map<str_t, MemoryShare *>;
    using BankList   = std::map<str_t, MemoryBank *>;
}