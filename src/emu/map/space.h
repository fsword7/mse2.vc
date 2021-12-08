// space.h - Address space package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    Dec 7, 2021

class ProcessorDevice;

namespace aspace
{
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
        AddressSpace() = default;
        virtual ~AddressSpace() = default;

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

        // Unmapped/no-operation read/write access
        HandlerEntry *unmapRead = nullptr;
        HandlerEntry *unmapWrite = nullptr;
        HandlerEntry *nopRead = nullptr;
        HandlerEntry *nopWrite = nullptr;
    };
}