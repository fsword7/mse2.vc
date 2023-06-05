// space.h - Address Space (memory access) package
//
// Author:  Tim Stark
// Date:    May 14, 2023

#pragma once

namespace map
{
    using Constructor = NamedDelegate<void (AddressList &)>;

    class AddressConfig
    {
    public:
        AddressConfig(cchar_t *name, endian_t eType,
            uint16_t dWidth, uint16_t dRadix, uint16_t dPrecision, uint16_t bWidth,
            uint16_t aWidth, uint16_t aRadix, uint16_t aPrecision, int16_t aShift)
        : name(name), endianType(eType),
          dataWidth(dWidth), dataRadix(dRadix), dataPrecision(dPrecision), byteWidth(bWidth),
          addrWidth(aWidth), addrRadix(aRadix), addrPrecision(aPrecision), addrShift(aShift)
        { 
            assert (dWidth >= aShift);
        }
        
        ~AddressConfig() = default;

        inline endian_t getEndianType() const    { return endianType; }
        inline uint16_t getDataWidth() const     { return dataWidth; }
        inline uint16_t getDataRadix() const     { return dataRadix; }
        inline uint16_t getDataPrecision() const { return dataPrecision; }
        inline uint16_t getByteWidth() const     { return byteWidth; }
        inline uint16_t getAddrWidth() const     { return addrWidth; }
        inline uint16_t getAddrRadix() const     { return addrRadix; }
        inline uint16_t getAddrPrecision() const { return addrPrecision; }
        inline int16_t  getAddrShift() const     { return addrShift; }
        inline int16_t  getPageShift() const     { return pageShift; }

        // Internal address map list
        inline void setAddressMap(Constructor map) { imap = map; }
        inline Constructor getAddressMap() const   { return imap; }

        offs_t convertAddressToByte(offs_t offset) const
        {
            return (addrShift < 0) ? (offset << -addrShift) : (offset >> addrShift);
        }

        offs_t convertByteToAddress(offs_t bytes) const
        {
            return (addrShift >= 0) ? (bytes << addrShift) : (bytes >> -addrShift);
        }

        offs_t convertAddressToByteEnd(offs_t offset) const
        {
            return (addrShift < 0) ? ((offset << -addrShift) | ((1ull << -addrShift) - 1ull)) : (offset >> addrShift);
        }

        offs_t convertByteToAddressEnd(offs_t bytes) const
        {
            return (addrShift >= 0) ? ((bytes << addrShift) | ((1ull << addrShift) - 1ull)) : (bytes >> -addrShift);
        }

    private:
        cchar_t *name = nullptr;

        endian_t endianType = LittleEndian;
        uint16_t dataWidth = 0;
        uint16_t dataRadix = 0;
        uint16_t dataPrecision = 0;
        uint16_t byteWidth = 0;

        uint16_t addrWidth = 0;
        uint16_t addrRadix = 0;
        uint16_t addrPrecision = 0;
        int16_t  addrShift = 0;
        int16_t  pageShift = 0;

        // Internal mapping
        Constructor imap = Constructor();
    };

    struct AddressSpaceConfig
    {
        const AddressType    type;
        const AddressConfig *config;
    };

    using AddressConfigList = std::vector<AddressSpaceConfig>;
    using cAddressConfig = const AddressConfig;

    class AddressSpaceInstaller
    {
    protected:
        AddressSpaceInstaller() = default;
        virtual ~AddressSpaceInstaller() = default;

    public:
        enum AccessType { accRead, accWrite };

        // virtual void setUnmapped(offs_t addrStart, offs_t addrEnd, offs_t addrMirror, uint64_t flags, AccessType acc, bool quiet) = 0;

        // virtual void setMemorySpace(offs_t addrStart, offs_t addrEnd, offs_t addrMirror, uint8_t *data, AccessType acc) = 0;

        // virtual void setMemoryView(offs_t addrStart, offs_t addrEnd, offs_t addrMirror, MemoryView *view) = 0;
        // virtual void setMemoryBank(offs_t addrStart, offs_t addrEnd, offs_t addrMirror, MemoryBank *bank, uint64_t flags, AccessType acc) = 0;

        // virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, read8d_t handler) = 0;
        // virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, read8do_t handler) = 0;
        // virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, read8dom_t handler) = 0;

        // virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, read16d_t handler) = 0;
        // virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, read16do_t handler) = 0; 
        // virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, read16dom_t handler) = 0;

        // virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, read32d_t handler) = 0;
        // virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, read32do_t handler) = 0;
        // virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, read32dom_t handler) = 0;

        // virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, read64d_t handler) = 0;
        // virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, read64do_t handler) = 0;
        // virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, read64dom_t handler) = 0;


        // virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, write8d_t handler) = 0;
        // virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, write8do_t handler) = 0;
        // virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, write8dom_t handler) = 0;

        // virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, write16d_t handler) = 0;
        // virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, write16do_t handler) = 0; 
        // virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, write16dom_t handler) = 0;

        // virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, write32d_t handler) = 0;
        // virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, write32do_t handler) = 0;
        // virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, write32dom_t handler) = 0;

        // virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, write64d_t handler) = 0;
        // virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, write64do_t handler) = 0;
        // virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, write64dom_t handler) = 0;


    };

    class AddressSpace : public AddressSpaceInstaller
    {
    public:
        AddressSpace(MemoryManager &manager, diMemory &bus, AddressType space);
        virtual ~AddressSpace() = default;

        offs_t getAddrMask() const { return addrMask; }
        uint64_t getUnmapped() const { return unmapValue; }
        
        template <int dWidth, int aShift>
        HandlerReadUnmapped<dWidth, aShift> *getUnmappedRead() const 
        { 
            return static_cast<HandlerReadUnmapped<dWidth, aShift> *>(unmapRead);
        }

        template <int dWidth, int aShift>
        HandlerWriteUnmapped<dWidth, aShift> *getUnmappedWrite() const
        { 
            return static_cast<HandlerWriteUnmapped<dWidth, aShift> *>(unmapWrite);
        }
        
        // Setup initialization routines
        void prepare(UserConsole *user);
        void populate(UserConsole *user);

        void populateEntry(const AddressEntry *entry, AccessType acc);

        virtual std::pair<const void *, const void *> getSpecificDispatches() = 0;
        virtual std::pair<const void *, const void *> getCacheDispatches() = 0;
 
        template <int Level, int dWidth, int aShift, endian_t eType>
        void setSpecificMemory(MemoryAccessSpecific<Level, dWidth, aShift, eType> &memAccess)
        {
            std::pair<const void *, const void *> rw = getSpecificDispatches();

            memAccess.set(this, addrMask, rw.first, rw.second);
        }
        
        // template <int Level, int dWidth, int aShift, endian_t eType>
        // void setCacheMemory(MemoryAccessCache<Level, dWidth, aShift, eType> &memAccess)
        // {
        //     std::pair<const void *, const void *> rw = getCacheDispatches();

        //     memAccess.set(this, addrMask, rw.first, rw.second);
        // }

        // Virtual function calls
        virtual uint8_t  read8(offs_t addr, CPUDevice *cpu = nullptr) = 0;
        virtual uint16_t read16(offs_t addr, CPUDevice *cpu = nullptr) = 0;
        virtual uint16_t read16u(offs_t addr, CPUDevice *cpu = nullptr) = 0;
        virtual uint32_t read32(offs_t addr, CPUDevice *cpu = nullptr) = 0;
        virtual uint32_t read32u(offs_t addr, CPUDevice *cpu = nullptr) = 0;
        virtual uint64_t read64(offs_t addr, CPUDevice *cpu = nullptr) = 0;
        virtual uint64_t read64u(offs_t addr, CPUDevice *cpu = nullptr) = 0;

        virtual void write8(offs_t addr, uint8_t data, CPUDevice *cpu = nullptr) = 0;
        virtual void write16(offs_t addr, uint16_t data, CPUDevice *cpu = nullptr) = 0;
        virtual void write16u(offs_t addr, uint16_t data, CPUDevice *cpu = nullptr) = 0;
        virtual void write32(offs_t addr, uint32_t data, CPUDevice *cpu = nullptr) = 0;
        virtual void write32u(offs_t addr, uint32_t data, CPUDevice *cpu = nullptr) = 0;
        virtual void write64(offs_t addr, uint64_t data, CPUDevice *cpu = nullptr) = 0;
        virtual void write64u(offs_t addr, uint64_t data, CPUDevice *cpu = nullptr) = 0;

    protected:
        AddressType     space;
        cAddressConfig &config;
        Device         &device;
        MemoryManager  &manager;

        AddressList *map = nullptr;

        offs_t   addrMask = 0;
        uint64_t unmapValue = 0;

        // Unmapped/no-operation read/write access
        HandlerEntry *unmapRead = nullptr;
        HandlerEntry *unmapWrite = nullptr;
        HandlerEntry *nopRead = nullptr;
        HandlerEntry *nopWrite = nullptr;
    };
};