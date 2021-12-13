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
        using uintx_t = typename HandlerSize<dWidth>::uinttx_t;

        HandlerReadDispatch(AddresSpace *space, const HandlerEntry::range &init,
            HandlerRead<dWidth, aShift> *handler = nullptr)
        : HandlerRead<dWidth, aShift>(space, HandlerEntry::heDispatch)
        {
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

        HandlerRead<dWidth, aShift> *const *getDispatch() const { return dispatch; }

        std::string getName() { return "dispatch"; }

        uintx_t read(offs_t offset, uintx_t mask) const
        {
            offs_t off = (offset >> lowBits) & bitMask;
            assert(dispatch[off] != nullptr);
            return dispatch[off]->read(offset, mask);
        }

    protected:
        static constexpr int      level    = determineDispatchLevel(highBits);
        static constexpr uint64_t lowBits  = determineDispatchLowBits(highBits, dWidth, aShift);
        static constexpr uint64_t bitCount = highBits > lowBits ? highBits - lowBits : 0;
        static constexpr uint64_t count    = 1ull << bitCount;

        static constexpr offs_t   bitMask  = makeBitmap<offs_t>(bitCount);
        static constexpr offs_t   lowMask  = maskBitmap<offs_t>(lowBits);
        static constexpr offs_t   highMask = makeBitmap<offs_t>(highBits) ^ lowMask;
        static constexpr offs_t   upMask   = ~makeBitmap<offs_t>(highBits);

        HandlerRead<dWidth, aShift> *dispatch[count];
        HandlerEntry::range ranges[count];
    };
}