// mview.cpp - Memory View (switching I/O and memory banks)
//
// Author:  Tim Stark
// Date:    1/1/22

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/memmgr.h"

#include "emu/map/hedr.h"
#include "emu/map/hedw.h"

#include "emu/map/hea.h"
#include "emu/map/hem.h"
#include "emu/map/hedp.h"

#include "emu/map/addrmap.h"

#include "emu/dimem.h"

namespace map
{
    AddressEntry &MemoryView::MemoryViewEntry::operator () (offs_t start, offs_t end)
    {
        assert(map != nullptr);
        return (*map)(start, end);
    }

    template <int Level, int dWidth, int aShift>
    class MemoryViewEntrySpecific : public MemoryView::MemoryViewEntry
    {
    public:
        MemoryViewEntrySpecific(const AddressConfig &config, MemoryManager &bus, MemoryView &view, int id)
        : MemoryViewEntry(config, bus, view, id)
        { }
        ~MemoryViewEntrySpecific() = default;

        void populate(AddressList *map = nullptr) override
        {

        }
    };
}