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

    public:
        Device &device;
        AddressList &map;

        offs_t addrStart;
        offs_t addrEnd;
    };

    class AddressList
    {
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