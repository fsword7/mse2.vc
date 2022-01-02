// mview.h - Memory View (switching I/O and memory banks)
//
// Author:  Tim Stark
// Date:    1/1/22

#pragma once

namespace map
{
    class MemoryView
    {
        template <int Level, int dWidth, int aShift, endian_t eType> friend class AddressSpaceSpecific;

        friend class MemoryViewEntry;
        friend class AddressEntry;
        friend class AddressList;

    public:
        class MemoryViewEntry : public AddressSpaceInstaller
        {
        public:
            virtual ~MemoryViewEntry() = default;

        protected:
            MemoryViewEntry(const AddressConfig &config, MemoryManager &bus, MemoryView &view, int id)
            : view(view), id(id)
            {

            }

            MemoryView &view;
            int id;
        };

        void init(offs_t sAddr, offs_t eAddr, cAddressConfig *nconfig)
        {
            assert (config == nullptr);

            config = nconfig;
            addrStart = sAddr;
            addrEnd = eAddr;
        }

    private:
        std::pair<HandlerEntry *, HandlerEntry *> makeHandlers(AddressSpace &space, offs_t addrStart, offs_t addrEnd)
        {
            return std::make_pair(handlerRead,  handlerWrite);
        }

        void makeDispatches()
        {
            // for (auto &entry : entries)
            //     entry->populate();
        }

    protected:
        cAddressConfig *config = nullptr;
        offs_t addrStart = 0;
        offs_t addrEnd   = 0;

        HandlerEntry *handlerRead = nullptr;
        HandlerEntry *handlerWrite = nullptr;

        std::vector<MemoryViewEntry> entries;
    };
}