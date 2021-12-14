// hedr.h - read handler entry package - dispatch
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/8/2021

#pragma once

namespace map
{
    template <int highBits, int dWidth, int aShift>
    class HandlerReadDispatch : public HandlerRead<dWidth, aShift>
    {
    public:
        using uintx_t = typename HandlerSize<dWidth>::uintx_t;

        HandlerReadDispatch(AddressSpace *space, const HandlerEntry::range &init,
            HandlerRead<dWidth, aShift> *handler = nullptr)
        : HandlerRead<dWidth, aShift>(space, HandlerEntry::heDispatch)
        {
            assert(lowBits >= 0);
            // Initialize dispatch table
            if (handler == nullptr)
                handler = space->getUnmappedRead<dWidth, aShift>();
            assert(handler != nullptr);
            handler->ref(count);
            for (int idx = 0; idx < count; idx++)
            {
                dispatch[idx] = handler;
                ranges[idx] = init;
            }
        }

        ~HandlerReadDispatch()
        {
            // Clear all dispatch table
            for (int idx = 0; idx < count; idx++)
                dispatch[idx]->unref();
        }

        static void validate() 
        {
            fmt::printf("Low bits<%d, %d>: %d\n", dWidth, aShift, lowBits);
        }

        HandlerRead<dWidth, aShift> *const *getDispatch() const { return dispatch; }

        std::string getName() { return "dispatch"; }

        uintx_t read(offs_t offset, uintx_t mask, ProcessorDevice *cpu) const override
        {
            offs_t off = ((lowBits > 0) ? (offset >> lowBits) : offset) & bitMask;
            assert(dispatch[off] != nullptr);
            return dispatch[off]->read(offset, mask);
        }

    protected:
        static constexpr const int      level    = determineDispatchLevel(highBits);
        static constexpr const int      lowBits  = determineDispatchLowBits(highBits, dWidth, aShift);
        static constexpr const int      bitCount = highBits > lowBits ? highBits - lowBits : 0;
        static constexpr const uint64_t count    = 1ull << bitCount;

        static constexpr const offs_t   bitMask  = makeBitmask<offs_t>(bitCount);
        static constexpr const offs_t   lowMask  = makeBitmask<offs_t>(lowBits);
        static constexpr const offs_t   highMask = makeBitmask<offs_t>(highBits) ^ lowMask;
        static constexpr const offs_t   upMask   = ~makeBitmask<offs_t>(highBits);

        HandlerRead<dWidth, aShift> *dispatch[count];
        HandlerEntry::range ranges[count];
    };
}