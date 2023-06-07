// space.cpp - Address Space (memory access) package
//
// Author:  Tim Stark
// Date:    May 14, 2023

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/memmgr.h"


#include "emu/dimem.h"

namespace map
{
    template <int Level, int dWidth, int aShift, endian_t eType>
    class AddressSpaceSpecific : public AddressSpace
    {
        using uintx_t = HandlerSize<dWidth>::uintx_t;
        using thisType = AddressSpaceSpecific<Level, dWidth, aShift, eType>;
        using nativeType = uintx_t;

        HandlerRead<dWidth, aShift> *rootRead = nullptr;
        HandlerWrite<dWidth, aShift> *rootWrite = nullptr;
        
        // For read/write access function calls
        const HandlerRead<dWidth, aShift> *const *dispatchRead = nullptr;
        const HandlerWrite<dWidth, aShift> *const *dispatchWrite = nullptr;

    public:
        AddressSpaceSpecific(MemoryManager &manager, diMemory &bus, AddressType space, int addrWidth)
        : AddressSpace(manager, bus, space)
        {
            assert(dWidth >= aShift);

            Device &dev = bus.getOwningDevice();

            // Set new unmapped/nop dispatch calls
            unmapRead = new HandlerReadUnmapped<dWidth, aShift>(this);
            unmapWrite = new HandlerWriteUnmapped<dWidth, aShift>(this);
            nopRead = new HandlerReadNop<dWidth, aShift>(this);
            nopWrite = new HandlerWriteNop<dWidth, aShift>(this);

            // Global address range for dispatch calls
            HandlerEntry::range r = { 0, static_cast<offs_t>(~0ull >> ((sizeof(offs_t) * 8) - addrWidth)) };

            std::cout << fmt::format("{}: Assigning dispatch calls at {}-bit addressing (data {} shift {})\n",
                device.getsDeviceName(), addrWidth, dWidth, aShift);

            switch (addrWidth)
            {
                // case 4:
                //     rootRead = new HandlerReadDispatch<4, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<4, dWidth, aShift>(this, r, nullptr);
                //     break;
                
                // case 5:
                //     rootRead = new HandlerReadDispatch<5, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<5, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 6:
                //     rootRead = new HandlerReadDispatch<6, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<6, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 7:
                //     rootRead = new HandlerReadDispatch<7, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<7, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 8:
                //     rootRead = new HandlerReadDispatch<8, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<8, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 9:
                //     rootRead = new HandlerReadDispatch<9, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<9, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 10:
                //     rootRead = new HandlerReadDispatch<10, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<10, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 11:
                //     rootRead = new HandlerReadDispatch<11, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<11, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 12:
                //     rootRead = new HandlerReadDispatch<12, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<12, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 13:
                //     rootRead = new HandlerReadDispatch<13, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<13, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 14:
                //     rootRead = new HandlerReadDispatch<14, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<14, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 15:
                //     rootRead = new HandlerReadDispatch<15, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<15, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 16:
                //     rootRead = new HandlerReadDispatch<16, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<16, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 17:
                //     rootRead = new HandlerReadDispatch<17, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<17, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 18:
                //     rootRead = new HandlerReadDispatch<18, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<18, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 19:
                //     rootRead = new HandlerReadDispatch<19, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<19, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 20:
                //     rootRead = new HandlerReadDispatch<20, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<20, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 21:
                //     rootRead = new HandlerReadDispatch<21, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<21, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 22:
                //     rootRead = new HandlerReadDispatch<22, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<22, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 23:
                //     rootRead = new HandlerReadDispatch<23, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<23, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 24:
                //     rootRead = new HandlerReadDispatch<24, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<24, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 25:
                //     rootRead = new HandlerReadDispatch<25, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<25, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 26:
                //     rootRead = new HandlerReadDispatch<26, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<26, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 27:
                //     rootRead = new HandlerReadDispatch<27, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<27, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 28:
                //     rootRead = new HandlerReadDispatch<28, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<28, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 29:
                //     rootRead = new HandlerReadDispatch<29, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<29, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 30:
                //     rootRead = new HandlerReadDispatch<30, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<30, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 31:
                //     rootRead = new HandlerReadDispatch<31, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<31, dWidth, aShift>(this, r, nullptr);
                //     break;

                // case 32:
                //     rootRead = new HandlerReadDispatch<32, dWidth, aShift>(this, r, nullptr);
                //     rootWrite = new HandlerWriteDispatch<32, dWidth, aShift>(this, r, nullptr);
                //     break;
            }

            // Assign dispatch table for direct read/write accesses
            // dispatchRead = rootRead->getDispatch();
            // dispatchWrite = rootWrite->getDispatch();
            
            std::cout << fmt::format("{}: Global address range {:0{}X} - {:0{}X} ({}-bit addressing)\n",
                dev.getsDeviceName(), r.start, config.getAddrPrecision(),
                r.end, config.getAddrPrecision(), addrWidth);
            std::cout << fmt::format("{}: Global address mask: {:0{}X} Unmapped value: {:02X}\n",
                dev.getsDeviceName(), addrMask, config.getAddrPrecision(), unmapValue);
        }

        std::pair<const void *, const void *> getSpecificDispatches() override
        {
            std::pair<const void *, const void *> rw;

            rw.first = dispatchRead;
            rw.second = dispatchWrite;

            return rw; 
        }

        std::pair<const void *, const void *> getCacheDispatches() override
        {
            std::pair<const void *, const void *> rw;

            rw.first = rootRead;
            rw.second = rootWrite;

            return rw;
        }

        uint8_t read8(offs_t addr, CPUDevice *cpu) override
        {
            // if (dWidth == 0)
            //     return readNative(addr, 0xFF, cpu);
            // return unmapValue;
            return 0;
        }

        uint16_t read16(offs_t addr, CPUDevice *cpu) override
        {
            return 0;
        }

        uint16_t read16u(offs_t addr, CPUDevice *cpu) override
        {
            return 0;
        }

        uint32_t read32(offs_t addr, CPUDevice *cpu) override
        {
            return 0;
        }
        
        uint32_t read32u(offs_t addr, CPUDevice *cpu) override
        {
            return 0;
        }

        uint64_t read64(offs_t addr, CPUDevice *cpu) override
        {
            return 0;
        }
        
        uint64_t read64u(offs_t addr, CPUDevice *cpu) override
        {
            return 0;
        }


        void write8(offs_t addr, uint8_t data, CPUDevice *cpu) override
        {
        }

        void write16(offs_t addr, uint16_t data, CPUDevice *cpu) override
        {
        }

        void write16u(offs_t addr, uint16_t data, CPUDevice *cpu) override
        {
        }

        void write32(offs_t addr, uint32_t data, CPUDevice *cpu) override
        {
        }
        
        void write32u(offs_t addr, uint32_t data, CPUDevice *cpu) override
        {
        }

        void write64(offs_t addr, uint64_t data, CPUDevice *cpu) override
        {
        }
        
        void write64u(offs_t addr, uint64_t data, CPUDevice *cpu) override
        {
        }


    };

    // ********

    constexpr std::initializer_list<AddressType> AddressTypes = { asProgram, asData, asIOPort };
    
    cchar_t *asDescrip[] = { "program", "data", "I/O port" };

    void MemoryManager::allocate(UserConsole *user, diMemory &bus)
    {
        Device &dev = bus.getOwningDevice();

        for (auto space : AddressTypes)
        {
            cAddressConfig *config = bus.getAddressConfig(space);
            if (config == nullptr)
                continue;
            
            std::cout << fmt::format("{}: Allocating {} address space...\n",
                dev.getsDeviceName(), asDescrip[space]);

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
            case 0x0000 | 0x0000 | 64+0: bus.allocate<AddressSpaceSpecific<0, 3, 0, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0000 | 64+1: bus.allocate<AddressSpaceSpecific<0, 3, 1, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0000 | 64+2: bus.allocate<AddressSpaceSpecific<0, 3, 2, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0000 | 64+3: bus.allocate<AddressSpaceSpecific<0, 3, 3, LittleEndian>>(*this, space); break;

            case 0x0000 | 0x0100 | 64+0: bus.allocate<AddressSpaceSpecific<1, 3, 0, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0100 | 64+1: bus.allocate<AddressSpaceSpecific<1, 3, 1, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0100 | 64+2: bus.allocate<AddressSpaceSpecific<1, 3, 2, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0100 | 64+3: bus.allocate<AddressSpaceSpecific<1, 3, 3, LittleEndian>>(*this, space); break;

            case 0x0000 | 0x0200 | 64+0: bus.allocate<AddressSpaceSpecific<2, 3, 0, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0200 | 64+1: bus.allocate<AddressSpaceSpecific<2, 3, 1, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0200 | 64+2: bus.allocate<AddressSpaceSpecific<2, 3, 2, LittleEndian>>(*this, space); break;
            case 0x0000 | 0x0200 | 64+3: bus.allocate<AddressSpaceSpecific<2, 3, 3, LittleEndian>>(*this, space); break;

            case 0x0400 | 0x0000 | 64+0: bus.allocate<AddressSpaceSpecific<0, 3, 0, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0000 | 64+1: bus.allocate<AddressSpaceSpecific<0, 3, 1, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0000 | 64+2: bus.allocate<AddressSpaceSpecific<0, 3, 2, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0000 | 64+3: bus.allocate<AddressSpaceSpecific<0, 3, 3, BigEndian>>(*this, space); break;

            case 0x0400 | 0x0100 | 64+0: bus.allocate<AddressSpaceSpecific<1, 3, 0, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0100 | 64+1: bus.allocate<AddressSpaceSpecific<1, 3, 1, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0100 | 64+2: bus.allocate<AddressSpaceSpecific<1, 3, 2, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0100 | 64+3: bus.allocate<AddressSpaceSpecific<1, 3, 3, BigEndian>>(*this, space); break;

            case 0x0400 | 0x0200 | 64+0: bus.allocate<AddressSpaceSpecific<2, 3, 0, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0200 | 64+1: bus.allocate<AddressSpaceSpecific<2, 3, 1, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0200 | 64+2: bus.allocate<AddressSpaceSpecific<2, 3, 2, BigEndian>>(*this, space); break;
            case 0x0400 | 0x0200 | 64+3: bus.allocate<AddressSpaceSpecific<2, 3, 3, BigEndian>>(*this, space); break;

            default:
                std::cout << fmt::format("{}: Invalid address space configuration: {}-bit data width, {}-bit address width (Level {}), {}-bit address shift\n",
                    dev.getsDeviceName(), config->getDataWidth(), config->getAddrWidth(), level, config->getAddrShift());
                break;
            }         
        }
    }
}