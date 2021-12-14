// hedw.h - Write Handler Entry - Dispatch
//
// Author:  Tim Stark
// Date:    12/14/2021

#pragma once

namespace map
{
    template <int highBits, int dWidth, int aShift>
    class HandlerWriteDispatch : public HandlerWrite<dWidth, aShift>
    {
    public:
        using uintx_t = typename HandlerSize<dWidth>::uintx_t;

        HandlerWriteDispatch(AddressSpace *space, const HandlerEntry::range &init,
            HandlerWrite<dWidth, aShift> *handler = nullptr)
        : HandlerWrite<dWidth, aShift>(space, HandlerEntry::heDispatch)
        {
            // Initialize dispatch table
            // if (handler == nullptr)
            //     handler = space->getUnmappedRead<dWidth, aShift>();
            assert(handler != nullptr);
            handler->ref(count);
            for (int idx = 0; idx < count; idx++)
            {
                dispatch[idx] = handler;
                ranges[idx] = init;
            }
        }

        ~HandlerWriteDispatch()
        {
            // Clear all dispatch table
            for (int idx = 0; idx < count; idx++)
                dispatch[idx]->unref();
        }

        HandlerWrite<dWidth, aShift> *const *getDispatch() const { return dispatch; }

        std::string getName() { return "dispatch"; }

        void write(offs_t offset, uintx_t data, uintx_t mask, ProcessorDevice *cpu) const override
        {
            offs_t off = (offset >> lowBits) & bitMask;
            assert(dispatch[off] != nullptr);
            dispatch[off]->write(offset, data, mask);
        }

    protected:
        static constexpr int      level    = determineDispatchLevel(highBits);
        static constexpr uint64_t lowBits  = determineDispatchLowBits(highBits, dWidth, aShift);
        static constexpr uint64_t bitCount = highBits > lowBits ? highBits - lowBits : 0;
        static constexpr uint64_t count    = 1ull << bitCount;

        static constexpr offs_t   bitMask  = makeBitmask<offs_t>(bitCount);
        static constexpr offs_t   lowMask  = makeBitmask<offs_t>(lowBits);
        static constexpr offs_t   highMask = makeBitmask<offs_t>(highBits) ^ lowMask;
        static constexpr offs_t   upMask   = ~makeBitmask<offs_t>(highBits);

        HandlerWrite<dWidth, aShift> *dispatch[count];
        HandlerEntry::range ranges[count];
    };
}