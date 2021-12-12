// addrmap.h - mapping address list package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

#pragma once

namespace aspace
{
    class AddressList;

    using Constructor = NamedDelegate<void (AddressList &)>;
    
    enum mapType
    {
        mapNone = 0,
        mapNOP,         // No operating - do nothing
        mapAccess,      // Accessible
        mapROMSpace,    // ROM space
        mapRAMSpace,    // RAM space
        mapIOPort,      // I/O port space
        mapBank,        // Memory bank space
        mapDelegate,    // Device delegate call ()
        mapDelegateO,   // Device delegate call (offset)
        mapDelegateOM,  // Device delegate call (offset, mask)
        mapVoid,        // Non-existent - exception trap
    };

    struct mapHandler
    {
        mapType     type = mapNone;
        uint16_t    bits = 0; // width bits, 0 = default
        ctag_t     *name = nullptr;
    };

    class AddressEntry
    {

    public:
        AddressEntry(Device &dev, AddressList &map, offs_t start, offs_t end);

    public:
        // Address entry information
        Device          &device;
        AddressList     &map;

        // Address parameters
        offs_t          addrStart  = 0;
        offs_t          addrEnd    = 0;
        offs_t          addrMask   = 0;
        offs_t          addrMirror = 0;

        // read/write access hanbdler parameters
        mapHandler      read, write;

        // Memory region parameters
        ctag_t         *regionName = nullptr;
        offs_t          regionOffset = 0;

        // Memory share paramters
        ctag_t         *shareName = nullptr;

        // Memory configuration
        uint8_t        *memData = nullptr;
        uint64_t        memSize = 0;
        uint64_t        maxSize = 0;
        bool            expFlag = false;

        // 8-bit device delegate calls
        read8d_t        read8;
        read8do_t       read8o;
        read8dom_t      read8om;

        write8d_t       write8;
        write8do_t      write8o;
        write8dom_t     write8om;

        // 16-bit device delegate calls
        read16d_t       read16;
        read16do_t      read16o;
        read16dom_t     read16om;

        write16d_t      write16;
        write16do_t     write16o;
        write16dom_t    write16om;

        // 32-bit device delegate calls
        read32d_t       read32;
        read32do_t      read32o;
        read32dom_t     read32om;

        write32d_t      write32;
        write32do_t     write32o;
        write32dom_t    write32om;

        // 64-bit device delegate calls
        read64d_t       read64;
        read64do_t      read64o;
        read64dom_t     read64om;

        write64d_t      write64;
        write64do_t     write64o;
        write64dom_t    write64om;

    public:
        // ROM/RAM access list
        AddressEntry &ram()                 { read.type = mapRAMSpace; write.type = mapRAMSpace; return *this; }
        AddressEntry &rom()                 { read.type = mapROMSpace; write.type = mapNOP;      return *this; }
        AddressEntry &ronly()               { read.type = mapRAMSpace;  return *this; }
        AddressEntry &wonly()               { write.type = mapRAMSpace; return *this; }

        AddressEntry &expandable()          { expFlag = true; return *this; }
        AddressEntry &unexpandable()        { expFlag = false; return *this; }
        AddressEntry &size(offs_t size)     { memSize = size; return *this; }

        AddressEntry &mirror(offs_t bits)   { addrMirror = bits; return *this; }
        AddressEntry &mask(uint64_t mask);

        AddressEntry &region(ctag_t *name, offs_t off = 0);
        AddressEntry &share(ctag_t *name)   { shareName = name; return *this; }

    };

    class AddressList
    {
        friend class AddressEntry;
        
    public:
        AddressList(Device &device, int space);
        ~AddressList();

        inline void setGlobalAddressMask(offs_t mask)   { gaddrMask = mask; }
        inline void setUnmappedValue(uint64_t val)      { unmapValue = val; }
        inline void setUnmappedHigh()                   { unmapValue = ~0ull; }
        inline void setUnmappedLow()                    { unmapValue = 0ull; }

        inline int getSize() const { return list.size(); }

        AddressEntry &operator () (offs_t start, offs_t end);

    private:
        Device &device;
        int addrSpace = 0;

        offs_t   gaddrMask = 0;
        uint64_t unmapValue = 0;

        std::vector<AddressEntry *> list;
    };
}