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

        private:
            Machine &sysMachine;

            // Memory libraries for all devices
            BlockList  blocks;
            RegionList regions;
            ShareList  shares;
            BankList   banks;
    };
}