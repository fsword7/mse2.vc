// space.cpp - Address space package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

#include "emu/core.h"
#include "emu/map/map.h"

namespace aspace
{

    template <int Level, int dWidth, int aShift, endian_t eType>
    class AddressSpaceSpecfic : public AddressSpace
    {
    private:
        HandlerRead<dWidth, aShift> *rootRead = nullptr;
        HandlerWrite<dWidth, aShift> *rootWrite = nullptr;

        // For read/write access function calls
        const HandlerRead<dWidth, aShift> *const *dispatchRead = nullptr;
        const HandlerWrite<dWidth, aShift> *const *dispatchWrite = nullptr;

    public:

        uint8_t read8(offs_t addr, ProcessorDevice *cpu)
        {
            return 0;
        }

        uint16_t read16(offs_t addr, ProcessorDevice *cpu)
        {
            return 0;
        }

        uint16_t read16u(offs_t addr, ProcessorDevice *cpu)
        {
            return 0;
        }

        uint32_t read32(offs_t addr, ProcessorDevice *cpu)
        {
            return 0;
        }
        
        uint32_t read32u(offs_t addr, ProcessorDevice *cpu)
        {
            return 0;
        }

        uint64_t read64(offs_t addr, ProcessorDevice *cpu)
        {
            return 0;
        }
        
        uint64_t read64u(offs_t addr, ProcessorDevice *cpu)
        {
            return 0;
        }


        void write8(offs_t addr, uint8_t data, ProcessorDevice *cpu)
        {
        }

        void write16(offs_t addr, uint16_t data, ProcessorDevice *cpu)
        {
        }

        void write16u(offs_t addr, uint16_t data, ProcessorDevice *cpu)
        {
        }

        void write32(offs_t addr, uint32_t data, ProcessorDevice *cpu)
        {
        }
        
        void write32u(offs_t addr, uint32_t data, ProcessorDevice *cpu)
        {
        }

        void write64(offs_t addr, uint64_t data, ProcessorDevice *cpu)
        {
        }
        
        void write64u(offs_t addr, uint64_t data, ProcessorDevice *cpu)
        {
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
}