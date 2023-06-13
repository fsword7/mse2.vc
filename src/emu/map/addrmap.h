// addrmap.h - Address Mapping package
//
// Author:  Tim Stark
// Date:    May 24, 2023

#pragma once

namespace map
{
    enum mapType
    {
        mapNone = 0,
        mapUnmapped,    // Unmapped
        mapNOP,         // No operating - do nothing
        mapAccess,      // Accessible
        mapROMSpace,    // ROM space
        mapRAMSpace,    // RAM space
        mapIOPort,      // I/O port space
        mapBank,        // Memory bank space
        mapView,        // Memory view space
        mapDelegate,    // Device delegate call ()
        mapDelegateO,   // Device delegate call (offset)
        mapDelegateOM,  // Device delegate call (offset, mask)
        mapVoid,        // Non-existent - exception trap
    };

    struct mapHandler
    {
        mapType     type = mapNone;
        uint16_t    bits = 0; // width bits, 0 = default
        cchar_t    *name = nullptr;
    };

    class AddressEntry
    {
    public:
        AddressEntry(Device &device, AddressList &map, offs_t start, offs_t end);

        AddressEntry  &ram()                    { read.type = mapRAMSpace; write.type = mapRAMSpace; return *this; }
        AddressEntry  &rom()                    { read.type = mapROMSpace; write.type = mapNOP;      return *this; }
        AddressEntry  &ronly()                  { read.type = mapRAMSpace;  return *this; }
        AddressEntry  &wonly()                  { write.type = mapRAMSpace; return *this; }
        
        AddressEntry  &noprw()                  { read.type = mapNOP; write.type = mapNOP; return *this; }
        AddressEntry  &nopr()                   { read.type = mapNOP;  return *this; }
        AddressEntry  &nopw()                   { write.type = mapNOP; return *this; }

        AddressEntry  &unmaprw()                { read.type = mapUnmapped; write.type = mapUnmapped; return *this; }
        AddressEntry  &unmapr()                 { read.type = mapUnmapped; return *this; }
        AddressEntry  &unmapw()                 { write.type = mapUnmapped; return *this; }

        // Set default memory size for memory space allocation
        AddressEntry  &expandable()             { expFlag = true; return *this; }
        AddressEntry  &unexpandable()           { expFlag = false; return *this; }
        AddressEntry  &allocate(offs_t size)    { addrSize = size; expFlag = true; return *this; }

        AddressEntry  &mirror(offs_t bits)      { addrMirror = bits; return *this; }
        AddressEntry  &mask(uint64_t mask);

        AddressEntry  &region(cchar_t *name, offs_t off = 0);
        AddressEntry  &share(cchar_t *name)     { shareName = name; return *this; }

        AddressEntry  &bankr(cchar_t *name)     { read.type = mapBank; read.name = name; return *this; }
        AddressEntry  &bankw(cchar_t *name)     { write.type = mapBank; write.name = name; return *this; }
        AddressEntry  &bankrw(cchar_t *name)    { bankr(name); bankw(name); return *this; }

        AddressEntry  &r(read8d_t func);
        AddressEntry  &r(read8do_t func);
        AddressEntry  &r(read8dom_t func);

        AddressEntry  &r(read16d_t func);
        AddressEntry  &r(read16do_t func);
        AddressEntry  &r(read16dom_t func);

        AddressEntry  &r(read32d_t func);
        AddressEntry  &r(read32do_t func);
        AddressEntry  &r(read32dom_t func);

        AddressEntry  &r(read64d_t func);
        AddressEntry  &r(read64do_t func);
        AddressEntry  &r(read64dom_t func);

        AddressEntry  &w(write8d_t func);
        AddressEntry  &w(write8do_t func);
        AddressEntry  &w(write8dom_t func);

        AddressEntry  &w(write16d_t func);
        AddressEntry  &w(write16do_t func);
        AddressEntry  &w(write16dom_t func);

        AddressEntry  &w(write32d_t func);
        AddressEntry  &w(write32do_t func);
        AddressEntry  &w(write32dom_t func);

        AddressEntry  &w(write64d_t func);
        AddressEntry  &w(write64do_t func);
        AddressEntry  &w(write64dom_t func);

    public:
        Device &device;
        AddressList &map;

        offs_t addrStart;
        offs_t addrEnd;
        offs_t addrMask;
        offs_t addrMirror;
        offs_t addrSize = 0;        // Expandable memory space
        bool   expFlag = false;

        mapHandler read, write;

        // Memory Region parameters
        cchar_t *regionName = nullptr;
        offs_t   regionOffset = 0;

        // Memeory Share parameters
        cchar_t *shareName = nullptr;

        // Memory parameters (allocated space)
        uint8_t   *memData = nullptr;
        size_t     memSize = 0;

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
    };

    class AddressList
    {
        friend class AddressEntry;
        friend class AddressSpace;

    public:
        AddressList(Device &dev, AddressType space);
        ~AddressList();

        AddressEntry &operator () (offs_t start, offs_t end);

    private:
        Device &device;
        Device *owner = nullptr;
        cAddressConfig *config = nullptr;
        int addrSpace = 0;

        offs_t gaddrMask = 0;
        uint64_t unmapValue = 0;

        std::vector<AddressEntry *> list;
    };
}