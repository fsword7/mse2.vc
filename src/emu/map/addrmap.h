// addrmap.h - mapping address list package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

#pragma once

namespace map
{
    class AddressList;
    class MemoryView;

    // using Constructor = NamedDelegate<void (AddressList &)>;
    
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

        MemoryView     *mview = nullptr;

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
        void view(MemoryView &nview);

        // ROM/RAM access list
        AddressEntry  &ram()                 { read.type = mapRAMSpace; write.type = mapRAMSpace; return *this; }
        AddressEntry  &rom()                 { read.type = mapROMSpace; write.type = mapNOP;      return *this; }
        AddressEntry  &ronly()               { read.type = mapRAMSpace;  return *this; }
        AddressEntry  &wonly()               { write.type = mapRAMSpace; return *this; }

        AddressEntry  &noprw()               { read.type = mapNOP; write.type = mapNOP; return *this; }
        AddressEntry  &nopr()                { read.type = mapNOP; return *this; }
        AddressEntry  &nopw()                { write.type = mapNOP; return *this; }

        AddressEntry  &unmaprw()             { read.type = mapUnmapped; write.type = mapUnmapped; return *this; }
        AddressEntry  &unmapr()              { read.type = mapUnmapped; return *this; }
        AddressEntry  &unmapw()              { write.type = mapUnmapped; return *this; }

        AddressEntry  &expandable()          { expFlag = true; return *this; }
        AddressEntry  &unexpandable()        { expFlag = false; return *this; }
        AddressEntry  &size(offs_t size)     { memSize = size; return *this; }

        AddressEntry  &mirror(offs_t bits)   { addrMirror = bits; return *this; }
        AddressEntry  &mask(uint64_t mask);

        AddressEntry  &region(ctag_t *name, offs_t off = 0);
        AddressEntry  &share(ctag_t *name)   { shareName = name; return *this; }

        AddressEntry  &bankr(ctag_t *name)   { read.type = mapBank; read.name = name; return *this; }
        AddressEntry  &bankw(ctag_t *name)   { write.type = mapBank; write.name = name; return *this; }
        AddressEntry  &bankrw(ctag_t *name)  { bankr(name); bankw(name); return *this; }

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

        // Implicit delegate function calls

        template <typename T, bool Required>
        static Device &findDevice(const DeviceFinder<T, Required> &finder)
        {
            Device *owner = &finder.getOwningDevice();
            assert(owner != nullptr);
            return *owner;
        }
        
        template <typename T, typename U>
        static T *makePointer(U &object)
        {
            if constexpr (std::is_convertible_v<std::add_pointer_t<U>, std::add_pointer_t<T> >)
                return mse_static_cast<T &>(object);
            else
                return &dynamic_cast<T &>(object);
        }

 
        // Implicit device - delegate calls
        template <typename T, typename rRet, typename... rArgs>
        AddressEntry &r(rRet (T::*read)(rArgs...), ctag_t *readName)
        {
            return r(makeDelegate(*makePointer<T>(device), read, readName));
        }

        template <typename T, typename wRet, typename... wArgs>
        AddressEntry &w(wRet (T::*write)(wArgs...), ctag_t *writeName)
        {
            return w(makeDelegate(*makePointer<T>(device), write, writeName));
        }

        template <typename T, typename rRet, typename... rArgs, typename U, typename wRet, typename... wArgs>
        AddressEntry &rw(rRet (T::*read)(rArgs...), ctag_t *readName,
                         wRet (U::*write)(wArgs...), ctag_t *writeName)
        {
            r(makeDelegate(*makePointer<T>(device), read, readName));
            w(makeDelegate(*makePointer<U>(device), write, writeName));
            return *this;
        }

        // Device reference - delegate calls
        template <typename T, typename U, typename rRet, typename... rArgs>
        AddressEntry &r(T &obj, rRet (U::*read)(rArgs...), ctag_t *readName)
        {
            return r(makeDelegate(*makePointer<U>(obj), read, readName));
        }

        template <typename T, typename U, typename wRet, typename... wArgs>
        AddressEntry &w(T &obj, wRet (U::*write)(wArgs...), ctag_t *writeName)
        {
            return w(makeDelegate(*makePointer<U>(obj), write, writeName));
        }

        template <typename T, typename U, typename rRet, typename... rArgs, typename V, typename wRet, typename... wArgs>
        AddressEntry &rw(T &obj, rRet (U::*read)(rArgs...), ctag_t *readName,
                                 wRet (V::*write)(wArgs...), ctag_t *writeName)
        {
            return r(makeDelegate(*makePointer<U>(obj), read, readName))
                  .w(makeDelegate(*makePointer<V>(obj), write, writeName));
        }

        // Device finder - delegate calls
        template <typename T, bool Required, typename U, typename rRet, typename... rArgs>
        AddressEntry &r(DeviceFinder<T, Required> &finder, rRet (U::*read)(rArgs...), ctag_t *readName)
        {
            Device &device(findDevice(finder));
            return r(makeDelegate(device, DEVICE_SELF, read, readName));
        }

        template <typename T, bool Required, typename U, typename wRet, typename... wArgs>
        AddressEntry &w(DeviceFinder<T, Required> &finder, wRet (U::*write)(wArgs...), ctag_t *writeName)
        {
            Device &device(findDevice(finder));
            return w(makeDelegate(device, DEVICE_SELF, write, writeName));
        }

        template <typename T, bool Required, typename U, typename rRet, typename... rArgs, typename V, typename wRet, typename... wArgs>
        AddressEntry &rw(DeviceFinder<T, Required> &finder,
            rRet (U::*read)(rArgs...), ctag_t *readName,
            wRet (V::*write)(wArgs...), ctag_t *writeName)
        {
            Device &device(findDevice(finder));
            return r(makeDelegate(device, DEVICE_SELF, read, readName))
                  .w(makeDelegate(device, DEVICE_SELF, write, writeName));
        }

    };

    class AddressList
    {
        friend class AddressEntry;
        friend class AddressSpace;
        
    public:
        AddressList(Device &device, map::AddressType space);
        ~AddressList();

        inline void setGlobalAddressMask(offs_t mask)   { gaddrMask = mask; }
        inline void setUnmappedValue(uint64_t val)      { unmapValue = val; }
        inline void setUnmappedHigh()                   { unmapValue = ~0ull; }
        inline void setUnmappedLow()                    { unmapValue = 0ull; }

        inline int getSize() const { return list.size(); }
        
        inline cAddressConfig *getConfig() const { return config; }

        AddressEntry &operator () (offs_t start, offs_t end);

    private:
        Device &device;
        // For mapping with owning device
        Device *owner = nullptr;
        cAddressConfig *config = nullptr;
        int addrSpace = 0;

        offs_t   gaddrMask = 0;
        uint64_t unmapValue = 0;

        std::vector<AddressEntry *> list;
    };
}