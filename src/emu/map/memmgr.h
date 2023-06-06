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

        // Memory block function calls
        uint8_t *allocateMemory(Device &owner, AddressType space, cstr_t &name,
            size_t bytes, int dWidth, endian_t eTYpe);

        // Memory region function calls
        MemoryRegion *allocateRegion(Device &owner, AddressType space, cstr_t &name,
            size_t bytes, int dWidth, endian_t eType);
        void releaeRegion(cstr_t &name);
        MemoryRegion *findRegion(cstr_t &name);

        // Memory share function calls
        MemoryShare *allocateShare(Device &owner, AddressType space, cstr_t &name,
            size_t bytes, int dWidth, endian_t eType);
        MemoryShare *findShare(cstr_t &name);

        // Memory block function calls
        MemoryBank *findBank(cstr_t &name);

    private:
        Machine &sysMachine;

        // Memory libraries for all devices
        BlockList  blocks;
        RegionList regions;
        ShareList  shares;
        BankList   banks;
    };

}