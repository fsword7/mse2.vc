// memmgr.h - Memory Manager package
//
// Author:  Tim Stark
// Date:    May 11, 2023

#pragma once

#include "emu/map/memlib.h"

namespace map
{
    class MemoryManager
    {
    public:
        MemoryManager(Machine &sys) : sysMachine(sys)
        { }
        ~MemoryManager() = default;

        inline Machine &getMachine() const      { return sysMachine; }

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