// memmgr.h - memory manager package
//
// Author:  Tim Stark
// Date:    12/10/2021

#pragma once

class Machine;
class diMemory;
class UserConsole;

#include "emu/map/memlib.h"

namespace map
{
    enum AddressType;

    class MemoryManager
    {
        public:
            MemoryManager(Machine &sys) : sysMachine(sys) {}
            ~MemoryManager() = default;

            inline Machine &getMachine() const       { return sysMachine; }

            inline BlockList &getBlockList()     { return blocks; }
            inline RegionList &getRegionList()   { return regions; }
            inline ShareList &getShareList()     { return shares; }
            inline BankList &getBankList()       { return banks; }

            void init(UserConsole *user);
            void allocate(UserConsole *user, diMemory &bus);

            // Memory block function calls
            uint8_t *allocateMemory(Device &owner, AddressType space, cstag_t &name,
                size_t bytes, int dWidth, endian_t eTYpe);

            // Memory region function calls
            MemoryRegion *allocateRegion(Device &owner, AddressType space, cstag_t &name,
                size_t bytes, int dWidth, endian_t eType);
            void releaeRegion(cstag_t &name);
            MemoryRegion *findRegion(cstag_t &name);

            // Memory share function calls
            MemoryShare *allocateShare(Device &owner, AddressType space, cstag_t &name,
                size_t bytes, int dWidth, endian_t eType);
            MemoryShare *findShare(cstag_t &name);

            // Memory block function calls
            MemoryBank *findBank(cstag_t &name);

        private:
            Machine &sysMachine;

            // Memory libraries for all devices
            BlockList  blocks;
            RegionList regions;
            ShareList  shares;
            BankList   banks;
    };
}