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
                std::cout << fmt::format("{}: Invalid address configuration - address {} width {} shift\n",
                    dev.getsDeviceName(), config->getAddrWidth(), config->getAddrShift());
                break;
            }         
        }
    }
}