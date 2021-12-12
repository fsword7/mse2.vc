// space.h - Address space package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    Dec 7, 2021

#pragma once

class ProcessorDevice;
class UserConsole;
class diMemory;

namespace aspace
{
    class MemoryManager;
    class AddressList;

    enum AddressType
    {
        asProgram = 0,  // Program address space
        asData,         // Data address space
        asIOPort        // I/O port address space
    };
    
    class AddressConfig
    {
    public:
        AddressConfig(ctag_t *name, endian_t eType,
            uint16_t dWidth, uint16_t dRadix, uint16_t dPrecision, uint16_t bWidth,
            uint16_t aWidth, uint16_t aRadix, uint16_t aPrecision, int16_t aShift)
        : name(name), endianType(eType),
          dataWidth(dWidth), dataRadix(dRadix), dataPrecision(dPrecision), byteWidth(bWidth),
          addrWidth(aWidth), addrRadix(aRadix), addrPrecision(aPrecision), addrShift(aShift)
        { }
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

    private:
        ctag_t *name = nullptr;

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
        virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const read8d_t &handler) = 0;
        virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const read8do_t &handler) = 0;
        virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const read8dom_t &handler) = 0;

        virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const read16d_t &handler) = 0;
        virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const read16do_t &handler) = 0; 
        virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const read16dom_t &handler) = 0;

        virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const read32d_t &handler) = 0;
        virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const read32do_t &handler) = 0;
        virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const read32dom_t &handler) = 0;

        virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const read64d_t &handler) = 0;
        virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const read64do_t &handler) = 0;
        virtual void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const read64dom_t &handler) = 0;


        virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const write8d_t &handler) = 0;
        virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const write8do_t &handler) = 0;
        virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const write8dom_t &handler) = 0;

        virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const write16d_t &handler) = 0;
        virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const write16do_t &handler) = 0; 
        virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const write16dom_t &handler) = 0;

        virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const write32d_t &handler) = 0;
        virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const write32do_t &handler) = 0;
        virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const write32dom_t &handler) = 0;

        virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const write64d_t &handler) = 0;
        virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const write64do_t &handler) = 0;
        virtual void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const write64dom_t &handler) = 0;


    };

    class AddressSpace : public AddressSpaceInstaller
    {
    public:
        AddressSpace(MemoryManager &manager, diMemory &bus, AddressType space);
        virtual ~AddressSpace() = default;

        // Setup initialization routines
        void prepare(UserConsole *user);
        void populate(UserConsole *user);

    protected:
        // Virtual function calls
        virtual uint8_t  read8(offs_t addr, ProcessorDevice *cpu = nullptr) = 0;
        virtual uint16_t read16(offs_t addr, ProcessorDevice *cpu = nullptr) = 0;
        virtual uint16_t read16u(offs_t addr, ProcessorDevice *cpu = nullptr) = 0;
        virtual uint32_t read32(offs_t addr, ProcessorDevice *cpu = nullptr) = 0;
        virtual uint32_t read32u(offs_t addr, ProcessorDevice *cpu = nullptr) = 0;
        virtual uint64_t read64(offs_t addr, ProcessorDevice *cpu = nullptr) = 0;
        virtual uint64_t read64u(offs_t addr, ProcessorDevice *cpu = nullptr) = 0;

        virtual void write8(offs_t addr, uint8_t data, ProcessorDevice *cpu = nullptr) = 0;
        virtual void write16(offs_t addr, uint16_t data, ProcessorDevice *cpu = nullptr) = 0;
        virtual void write16u(offs_t addr, uint16_t data, ProcessorDevice *cpu = nullptr) = 0;
        virtual void write32(offs_t addr, uint32_t data, ProcessorDevice *cpu = nullptr) = 0;
        virtual void write32u(offs_t addr, uint32_t data, ProcessorDevice *cpu = nullptr) = 0;
        virtual void write64(offs_t addr, uint64_t data, ProcessorDevice *cpu = nullptr) = 0;
        virtual void write64u(offs_t addr, uint64_t data, ProcessorDevice *cpu = nullptr) = 0;

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
}