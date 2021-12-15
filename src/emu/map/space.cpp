// space.cpp - Address space package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/memmgr.h"
#include "emu/map/hedr.h"
#include "emu/map/hedw.h"
#include "emu/map/hea.h"
#include "emu/map/hem.h"

#include "emu/dimem.h"

namespace map
{

    template <int Level, int dWidth, int aShift, endian_t eType>
    class AddressSpaceSpecific : public AddressSpace
    {
    private:
        HandlerRead<dWidth, aShift> *rootRead = nullptr;
        HandlerWrite<dWidth, aShift> *rootWrite = nullptr;

        // For read/write access function calls
        const HandlerRead<dWidth, aShift> *const *dispatchRead = nullptr;
        const HandlerWrite<dWidth, aShift> *const *dispatchWrite = nullptr;

    public:
        AddressSpaceSpecific(MemoryManager &manager, diMemory &bus, AddressType space, int addrWidth)
        : AddressSpace(manager, bus, space)
        {
            Device &dev = bus.getOwningDevice();

            // Set new unmapped/nop dispatch calls
            unmapRead = new HandlerReadUnmapped<dWidth, aShift>(this);
            unmapWrite = new HandlerWriteUnmapped<dWidth, aShift>(this);
            nopRead = new HandlerReadNop<dWidth, aShift>(this);
            nopWrite = new HandlerWriteNop<dWidth, aShift>(this);

            // Global address range for dispatch calls
            HandlerEntry::range r = { 0, static_cast<offs_t>(~0ull >> ((sizeof(offs_t) * 8) - addrWidth)) };

            fmt::printf("%s: Assigning dispatch calls at %d-bit addressing (data %d shift %d)\n",
                device.getDeviceName(), addrWidth, dWidth, aShift);

            switch (addrWidth)
            {
                case 4:
                    rootRead = new HandlerReadDispatch<4, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<4, dWidth, aShift>(this, r, nullptr);
                    break;
                
                case 5:
                    rootRead = new HandlerReadDispatch<5, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<5, dWidth, aShift>(this, r, nullptr);
                    break;

                case 6:
                    rootRead = new HandlerReadDispatch<6, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<6, dWidth, aShift>(this, r, nullptr);
                    break;

                case 7:
                    rootRead = new HandlerReadDispatch<7, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<7, dWidth, aShift>(this, r, nullptr);
                    break;

                case 8:
                    rootRead = new HandlerReadDispatch<8, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<8, dWidth, aShift>(this, r, nullptr);
                    break;

                case 9:
                    rootRead = new HandlerReadDispatch<9, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<9, dWidth, aShift>(this, r, nullptr);
                    break;

                case 10:
                    rootRead = new HandlerReadDispatch<10, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<10, dWidth, aShift>(this, r, nullptr);
                    break;

                case 11:
                    rootRead = new HandlerReadDispatch<11, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<11, dWidth, aShift>(this, r, nullptr);
                    break;

                case 12:
                    rootRead = new HandlerReadDispatch<12, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<12, dWidth, aShift>(this, r, nullptr);
                    break;

                case 13:
                    rootRead = new HandlerReadDispatch<13, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<13, dWidth, aShift>(this, r, nullptr);
                    break;

                case 14:
                    rootRead = new HandlerReadDispatch<14, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<14, dWidth, aShift>(this, r, nullptr);
                    break;

                case 15:
                    rootRead = new HandlerReadDispatch<15, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<15, dWidth, aShift>(this, r, nullptr);
                    break;

                case 16:
                    rootRead = new HandlerReadDispatch<16, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<16, dWidth, aShift>(this, r, nullptr);
                    break;

                case 17:
                    rootRead = new HandlerReadDispatch<17, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<17, dWidth, aShift>(this, r, nullptr);
                    break;

                case 18:
                    rootRead = new HandlerReadDispatch<18, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<18, dWidth, aShift>(this, r, nullptr);
                    break;

                case 19:
                    rootRead = new HandlerReadDispatch<19, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<19, dWidth, aShift>(this, r, nullptr);
                    break;

                case 20:
                    rootRead = new HandlerReadDispatch<20, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<20, dWidth, aShift>(this, r, nullptr);
                    break;

                case 21:
                    rootRead = new HandlerReadDispatch<21, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<21, dWidth, aShift>(this, r, nullptr);
                    break;

                case 22:
                    rootRead = new HandlerReadDispatch<22, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<22, dWidth, aShift>(this, r, nullptr);
                    break;

                case 23:
                    rootRead = new HandlerReadDispatch<23, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<23, dWidth, aShift>(this, r, nullptr);
                    break;

                case 24:
                    rootRead = new HandlerReadDispatch<24, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<24, dWidth, aShift>(this, r, nullptr);
                    break;

                case 25:
                    rootRead = new HandlerReadDispatch<25, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<25, dWidth, aShift>(this, r, nullptr);
                    break;

                case 26:
                    rootRead = new HandlerReadDispatch<26, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<26, dWidth, aShift>(this, r, nullptr);
                    break;

                case 27:
                    rootRead = new HandlerReadDispatch<27, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<27, dWidth, aShift>(this, r, nullptr);
                    break;

                case 28:
                    rootRead = new HandlerReadDispatch<28, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<28, dWidth, aShift>(this, r, nullptr);
                    break;

                case 29:
                    rootRead = new HandlerReadDispatch<29, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<29, dWidth, aShift>(this, r, nullptr);
                    break;

                case 30:
                    rootRead = new HandlerReadDispatch<30, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<30, dWidth, aShift>(this, r, nullptr);
                    break;

                case 31:
                    rootRead = new HandlerReadDispatch<31, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<31, dWidth, aShift>(this, r, nullptr);
                    break;

                case 32:
                    rootRead = new HandlerReadDispatch<32, dWidth, aShift>(this, r, nullptr);
                    rootWrite = new HandlerWriteDispatch<32, dWidth, aShift>(this, r, nullptr);
                    break;
            }

            fmt::printf("%s: Global address range %0*llX - %0*llX (%d-bit addressing)\n",
                dev.getDeviceName(), config.getAddrPrecision(), r.start,
                config.getAddrPrecision(), r.end, addrWidth);
            fmt::printf("%s: Global address mask: %0*llX Unmapped value: %02X\n",
                dev.getDeviceName(), config.getAddrPrecision(), addrMask, unmapValue);
        }

        uint8_t read8(offs_t addr, ProcessorDevice *cpu) override
        {
            return 0;
        }

        uint16_t read16(offs_t addr, ProcessorDevice *cpu) override
        {
            return 0;
        }

        uint16_t read16u(offs_t addr, ProcessorDevice *cpu) override
        {
            return 0;
        }

        uint32_t read32(offs_t addr, ProcessorDevice *cpu) override
        {
            return 0;
        }
        
        uint32_t read32u(offs_t addr, ProcessorDevice *cpu) override
        {
            return 0;
        }

        uint64_t read64(offs_t addr, ProcessorDevice *cpu) override
        {
            return 0;
        }
        
        uint64_t read64u(offs_t addr, ProcessorDevice *cpu) override
        {
            return 0;
        }


        void write8(offs_t addr, uint8_t data, ProcessorDevice *cpu) override
        {
        }

        void write16(offs_t addr, uint16_t data, ProcessorDevice *cpu) override
        {
        }

        void write16u(offs_t addr, uint16_t data, ProcessorDevice *cpu) override
        {
        }

        void write32(offs_t addr, uint32_t data, ProcessorDevice *cpu) override
        {
        }
        
        void write32u(offs_t addr, uint32_t data, ProcessorDevice *cpu) override
        {
        }

        void write64(offs_t addr, uint64_t data, ProcessorDevice *cpu) override
        {
        }
        
        void write64u(offs_t addr, uint64_t data, ProcessorDevice *cpu) override
        {
        }

        void convertAddressMirror(offs_t start, offs_t end, offs_t mirror,
            offs_t &nstart, offs_t &nend, offs_t &nmask, offs_t &nmirror)
        {
            offs_t lowMask    = (config.getDataWidth() >> config.getAddrShift()) - 1;
            offs_t setBits    = start | end;
            offs_t changeBits = start ^ end;

            changeBits |= changeBits >> 1;
            changeBits |= changeBits >> 2;
            changeBits |= changeBits >> 4;
            changeBits |= changeBits >> 8;
            changeBits |= changeBits >> 16;

            nstart  = start;
            nend    = end;
            nmask   = changeBits;
            nmirror = mirror;
        }

        void setMemorySpace(offs_t addrStart, offs_t addrEnd, offs_t addrMirror, uint8_t *data, AccessType acc) override
        {
            assert(data != nullptr);

            offs_t nstart, nend, nmask, nmirror;
            convertAddressMirror(addrStart, addrEnd, addrMirror, nstart, nend, nmask, nmirror);

            if (acc == accRead)
            {
                auto handler = new HandlerReadMemory<dWidth, aShift>(this, data);
                handler->setAddressSpace(nstart, nend);
                rootRead->populate(nstart, nend, nmirror, handler);
            }

            if (acc == accWrite)
            {
                auto handler = new HandlerWriteMemory<dWidth, aShift>(this, data);
                handler->setAddressSpace(nstart, nend);
                rootWrite->populate(nstart, nend, nmirror, handler);
            }
        }

        // 8-bit read device delegate call setup
        void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const read8d_t &handler) override
        {
            setReadHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const read8do_t &handler) override
        {
            setReadHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const read8dom_t &handler) override
        {
            setReadHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        // 16-bit read device delegate call setup 
        void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const read16d_t &handler) override
        {
            setReadHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const read16do_t &handler) override
        {
            setReadHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const read16dom_t &handler) override
        {
            setReadHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        // 32-bit read device delegate call setup
        void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const read32d_t &handler) override
        {
            setReadHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const read32do_t &handler) override
        {
            setReadHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const read32dom_t &handler) override
        {
            setReadHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        // 64-bit read device delegate call setup
        void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const read64d_t &handler) override
        {
            setReadHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const read64do_t &handler) override
        {
            setReadHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        void setReadHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const read64dom_t &handler) override
        {
            setReadHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        // 8-bit write device delegate call setup
        void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const write8d_t &handler) override
        {
            setWriteHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const write8do_t &handler) override
        {
            setWriteHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const write8dom_t &handler) override
        {
            setWriteHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        // 16-bit write device delegate call setup
        void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const write16d_t &handler) override
        {
            setWriteHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const write16do_t &handler) override
        {
            setWriteHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const write16dom_t &handler) override
        {
            setWriteHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        // 32-bit write device delegate call setup
        void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const write32d_t &handler) override
        {
            setWriteHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const write32do_t &handler) override
        {
            setWriteHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const write32dom_t &handler) override
        {
            setWriteHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        // 64-bit write device delegate call setup
        void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const write64d_t &handler) override
        {
            setWriteHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const write64do_t &handler) override
        {
            setWriteHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        void setWriteHandler(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror,  const write64dom_t &handler) override
        {
            setWriteHandleri(addrStart, addrEnd, addrMask, addrMirror, handler);
        }

        template <typename Read>
        void setReadHandleri(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const Read &rhandler)
        {

        }

        template <typename Write>
        void setWriteHandleri(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const Write &whandler)
        {
            
        }

        template <int accWidth, typename Read>
        void setReadHandlerHelper(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const Read &rhandler)
        {

        }

        template <int accWidth, typename Write>
        void setWriteHandlerHelper(offs_t addrStart, offs_t addrEnd, offs_t addrMask, offs_t addrMirror, const Write &whandler)
        {
            
        }
    };

    // ********

    constexpr std::initializer_list<AddressType> AddressTypes = { asProgram, asData, asIOPort };
    
    ctag_t *asDescrip[] = { "program", "data", "I/O port" };

    void MemoryManager::allocate(UserConsole *user, diMemory &bus)
    {
        Device &dev = bus.getOwningDevice();

        for (auto space : AddressTypes)
        {
            cAddressConfig *config = bus.getAddressConfig(space);
            if (config == nullptr)
                continue;

            fmt::printf("%s: Allocating %s address space...\n",
                dev.getDeviceName(), asDescrip[space]);

            int level = determineDispatchLevel(config->getAddrWidth());
            int eType = config->getEndianType() == BigEndian ? 0x0400 : 0x0000;

            switch (eType | (level << 8) | config->getDataWidth() | (config->getAddrShift() & 0x3))
            {
            // 8-bit data access
            case 0x0000 | 0x0000 | 8:    bus.allocate<AddressSpaceSpecific<0, 0, 0, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0100 | 8:    bus.allocate<AddressSpaceSpecific<1, 0, 0, LittleEndian>>(*this, space); break;
            case 0x0400 | 0x0000 | 8:    bus.allocate<AddressSpaceSpecific<0, 0, 0, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0100 | 8:    bus.allocate<AddressSpaceSpecific<1, 0, 0, BigEndian>>(*this, space); break;

            // 16-bit data access
            case 0x0000 | 0x0000 | 16+0: bus.allocate<AddressSpaceSpecific<0, 1, 0, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0000 | 16+1: bus.allocate<AddressSpaceSpecific<0, 1, 1, LittleEndian>>(*this, space); break;

            case 0x0000 | 0x0100 | 16+0: bus.allocate<AddressSpaceSpecific<1, 1, 0, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0100 | 16+1: bus.allocate<AddressSpaceSpecific<1, 1, 1, LittleEndian>>(*this, space); break;

            case 0x0400 | 0x0000 | 16+0: bus.allocate<AddressSpaceSpecific<0, 1, 0, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0000 | 16+1: bus.allocate<AddressSpaceSpecific<0, 1, 1, BigEndian>>(*this, space); break;

            case 0x0400 | 0x0100 | 16+0: bus.allocate<AddressSpaceSpecific<1, 1, 0, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0100 | 16+1: bus.allocate<AddressSpaceSpecific<1, 1, 1, BigEndian>>(*this, space); break;

            // 32-bit data access
            case 0x0000 | 0x0000 | 32+0: bus.allocate<AddressSpaceSpecific<0, 2, 0, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0000 | 32+1: bus.allocate<AddressSpaceSpecific<0, 2, 1, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0000 | 32+2: bus.allocate<AddressSpaceSpecific<0, 2, 2, LittleEndian>>(*this, space); break;

            case 0x0000 | 0x0100 | 32+0: bus.allocate<AddressSpaceSpecific<1, 2, 0, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0100 | 32+1: bus.allocate<AddressSpaceSpecific<1, 2, 1, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0100 | 32+2: bus.allocate<AddressSpaceSpecific<1, 2, 2, LittleEndian>>(*this, space); break;

            case 0x0400 | 0x0000 | 32+0: bus.allocate<AddressSpaceSpecific<0, 2, 0, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0000 | 32+1: bus.allocate<AddressSpaceSpecific<0, 2, 1, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0000 | 32+2: bus.allocate<AddressSpaceSpecific<0, 2, 2, BigEndian>>(*this, space); break;

            case 0x0400 | 0x0100 | 32+0: bus.allocate<AddressSpaceSpecific<1, 2, 0, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0100 | 32+1: bus.allocate<AddressSpaceSpecific<1, 2, 1, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0100 | 32+2: bus.allocate<AddressSpaceSpecific<1, 2, 2, BigEndian>>(*this, space); break;

            // 64-bit data access
            case 0x0000 | 0x0000 | 64+0: bus.allocate<AddressSpaceSpecific<0, 2, 0, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0000 | 64+1: bus.allocate<AddressSpaceSpecific<0, 2, 1, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0000 | 64+2: bus.allocate<AddressSpaceSpecific<0, 2, 2, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0000 | 64+3: bus.allocate<AddressSpaceSpecific<0, 2, 3, LittleEndian>>(*this, space); break;

            case 0x0000 | 0x0100 | 64+0: bus.allocate<AddressSpaceSpecific<1, 2, 0, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0100 | 64+1: bus.allocate<AddressSpaceSpecific<1, 2, 1, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0100 | 64+2: bus.allocate<AddressSpaceSpecific<1, 2, 2, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0100 | 64+3: bus.allocate<AddressSpaceSpecific<1, 2, 3, LittleEndian>>(*this, space); break;

            case 0x0400 | 0x0000 | 64+0: bus.allocate<AddressSpaceSpecific<0, 2, 0, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0000 | 64+1: bus.allocate<AddressSpaceSpecific<0, 2, 1, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0000 | 64+2: bus.allocate<AddressSpaceSpecific<0, 2, 2, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0000 | 64+3: bus.allocate<AddressSpaceSpecific<0, 2, 3, BigEndian>>(*this, space); break;

            case 0x0400 | 0x0100 | 64+0: bus.allocate<AddressSpaceSpecific<1, 2, 0, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0100 | 64+1: bus.allocate<AddressSpaceSpecific<1, 2, 1, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0100 | 64+2: bus.allocate<AddressSpaceSpecific<1, 2, 2, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0100 | 64+3: bus.allocate<AddressSpaceSpecific<1, 2, 3, BigEndian>>(*this, space); break;

            default:
                fmt::printf("%s: Invalid address configuration - address %d width %d shift\n",
                    dev.getDeviceName(), config->getAddrWidth(), config->getAddrShift());
                break;
            }
        }
    }
}
 