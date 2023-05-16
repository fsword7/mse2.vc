// he.h - Header Entry package
//
// Author:  Tim Stark
// Date:    May 15, 2023

// Virtual address bits
//
//  6 6 6 6 5 5 5 5 5 5 5 5 5 5 4 4 4 4 4 4 4 4 4 4 3 3 3 3 3 3 3 3 3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |            Level 4            |            Level 3            |           Level 1             |           Level 0             |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3 3 3 4 4 4 4 4 4 4 4 4 4 5 5 5 5 5 5 5 5 5 5 6 6 6 6
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3

// High bit address for paging
#define PAG_LEVEL3   48
#define PAG_LEVEL2   32
#define PAG_LEVEL1   16
#define PAG_LEVEL0   0

namespace map
{
    // Data width - types
    template <int dWidth> struct HandlerSize { };
    template <> struct HandlerSize<0> { using uintx_t = uint8_t; };
    template <> struct HandlerSize<1> { using uintx_t = uint16_t; };
    template <> struct HandlerSize<2> { using uintx_t = uint32_t; };
    template <> struct HandlerSize<3> { using uintx_t = uint64_t; };

    constexpr int determineDispatchLevel(int aWidth)
    {
        return (aWidth > PAG_LEVEL3) ? 3 :
               (aWidth > PAG_LEVEL2) ? 2 :
               (aWidth > PAG_LEVEL1) ? 1 :
               0;
    }

    constexpr int determineDispatchLowBits(int aWidth, int dWidth, int aShift)
    {
        return (aWidth > PAG_LEVEL3) ? PAG_LEVEL3 :
               (aWidth > PAG_LEVEL2) ? PAG_LEVEL2 :
               (aWidth > PAG_LEVEL1) ? PAG_LEVEL1 :
               (dWidth - aShift);
    }

    class HandlerEntry
    {
    public:
        static constexpr uint32_t heDispatch = 0x0000'0001;

        HandlerEntry(AddressSpace *space, uint32_t flags)
        : space(space), flags(flags), refCount(1)
        { }

		inline bool isDispatch() const { return flags & heDispatch; }

        inline void ref(int count = 1)
        {
            refCount += count;
        }

        inline void unref(int count = 1)
        {
            refCount -= count;
            if (refCount == 0)
                delete this;
        }

    protected:
        AddressSpace *space = nullptr;
        uint32_t      flags = 0;
        mutable int   refCount = 0;

    public:
        struct range
        {
            offs_t start;
            offs_t end;

            inline void set(offs_t _start, offs_t _end)
            {
                start = _start;
                end = _end;
            }

            inline void intersect(offs_t _start, offs_t _end)
            {
                if (_start > start)
                    start = _start;
                if (_end < end)
                    end = _end;
            }
        };
    };

    // template <int dWidth, int aShift>
    // class HandlerRead : public HandlerEntry
    // {
    // public:
    //     using uintx_t = typename HandlerSize<dWidth>::uintx_t;

    //     static constexpr offs_t nativeMask = (dWidth - aShift) >= 0 ? makeBitmask<offs_t>(dWidth - aShift) : 0;

    //     HandlerRead(AddressSpace *space, uint32_t flags)
    //     : HandlerEntry(space, flags)
    //     {
    //         assert(dWidth >= aShift);
    //     }

    //     virtual ~HandlerRead() = default;

    //     virtual HandlerRead<dWidth, aShift> *const *getDispatch() const { return nullptr; };

    //     virtual uintx_t read(offs_t offset, uintx_t mask, ProcessorDevice *cpu = nullptr) const = 0;
    //     virtual void *getAccess(offs_t offset) const { return nullptr; }

    //     inline void populate(offs_t sAddr, offs_t eAddr, offs_t mAddr, HandlerRead<dWidth, aShift> *handler)
	// 	{
	// 		sAddr &= ~nativeMask;
	// 		eAddr |= nativeMask;
	// 		if (mAddr != 0)
	// 			populateMirror(sAddr, eAddr, sAddr, eAddr, mAddr, handler);
	// 		else
	// 			populateNoMirror(sAddr, eAddr, sAddr, eAddr, handler);
	// 	}

	// 	virtual void populateSubdispatchMirror(offs_t entry, offs_t sAddr, offs_t eAddr,
    //         offs_t sOrgin, offs_t eOrgin, offs_t mirror, HandlerRead<dWidth, aShift> *handler) {}
	// 	virtual void populateMirror(offs_t sAddr, offs_t eAddr,
    //         offs_t sOrgin, offs_t eOrgin, offs_t mirror, HandlerRead<dWidth, aShift> *handler) {}
	// 	virtual void populateSubdispatchNoMirror(offs_t entry, offs_t sAddr, offs_t eAddr,
    //         offs_t sOrgin, offs_t eOrgin, HandlerRead<dWidth, aShift> *handler) {}
	// 	virtual void populateNoMirror(offs_t sAddr, offs_t eAddr,
    //         offs_t sOrgin, offs_t eOrgin, HandlerRead<dWidth, aShift> *handler) {}
    // };

    // template <int dWidth, int aShift>
    // class HandlerWrite : public HandlerEntry
    // {
    // public:
    //     using uintx_t = typename HandlerSize<dWidth>::uintx_t;
        
    //     static constexpr offs_t nativeMask = (dWidth - aShift) >= 0 ? makeBitmask<offs_t>(dWidth - aShift) : 0;

    //     HandlerWrite(AddressSpace *space, uint32_t flags)
    //     : HandlerEntry(space, flags)
    //     { 
    //         assert(dWidth >= aShift);
    //     }
        
    //     virtual ~HandlerWrite() = default;

    //     virtual HandlerWrite<dWidth, aShift> *const *getDispatch() const { return nullptr; };

    //     virtual void write(offs_t offset, uintx_t data, uintx_t mask, ProcessorDevice *cpu = nullptr) const = 0;
    //     virtual void *getAccess(offs_t offset) const { return nullptr; }

    //     inline void populate(offs_t sAddr, offs_t eAddr, offs_t mAddr, HandlerWrite<dWidth, aShift> *handler)
	// 	{
	// 		sAddr &= ~nativeMask;
	// 		eAddr |= nativeMask;
	// 		if (mAddr != 0)
	// 			populateMirror(sAddr, eAddr, mAddr, sAddr, eAddr, handler);
	// 		else
	// 			populateNoMirror(sAddr, eAddr, sAddr, eAddr, handler);
	// 	}

	// 	virtual void populateSubdispatchMirror(offs_t entry, offs_t sAddr, offs_t eAddr, offs_t mAddr,
	// 		offs_t sOrgin, offs_t eOrgin, HandlerWrite<dWidth, aShift> *handler) {}
	// 	virtual void populateMirror(offs_t sAddr, offs_t eAddr, offs_t mAddr,
	// 		offs_t sOrgin, offs_t eOrgin, HandlerWrite<dWidth, aShift> *handler) {}
	// 	virtual void populateSubdispatchNoMirror(offs_t entry, offs_t sAddr, offs_t eAddr,
	// 		offs_t sOrgin, offs_t eOrgin, HandlerWrite<dWidth, aShift> *handler) {}
	// 	virtual void populateNoMirror(offs_t sAddr, offs_t eAddr,
	// 		offs_t sOrgin, offs_t eOrgin, HandlerWrite<dWidth, aShift> *handler) {}
    // };

}