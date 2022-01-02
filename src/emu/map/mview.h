// mview.h - Memory View (switching I/O and memory banks)
//
// Author:  Tim Stark
// Date:    1/1/22

#pragma once

namespace map
{
    class MemoryView
    {

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

    protected:
        cAddressConfig *config = nullptr;
        offs_t addrStart = 0;
        offs_t addrEnd   = 0;

        std::vector<MemoryViewEntry> entries;
    };
}