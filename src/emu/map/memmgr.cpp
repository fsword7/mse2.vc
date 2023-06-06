// memmgr.cpp - Memory Manager package
//
// Author:  Tim Stark
// Date:    May 11, 2023

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/machine.h"
#include "emu/dimem.h"

using namespace map;

void MemoryManager::init(UserConsole *user)
{
    std::cout << fmt::format("{}: Memory Management Initialiation...",
        sysMachine.getDeviceName());

    std::vector<diMemory *> list;

    list.clear();
    for (diMemory &bus : InterfaceIterator<diMemory>(*sysMachine.getSystemDevice()))
    {
        list.push_back(&bus);
        allocate(user, bus);
    }
}

// Memory block function call definitions

uint8_t *MemoryManager::allocateMemory(Device &owner, AddressType space, cstr_t &name,
    size_t bytes, int dWidth, endian_t eType)
{
    MemoryBlock *block = new MemoryBlock(name, bytes, dWidth, eType);

    if (block != nullptr)
        blocks.push_back(block);
    return block->getData();
}


// Memory region function call definitions

MemoryRegion *MemoryManager::allocateRegion(Device &owner, AddressType space, cstr_t &name,
    size_t bytes, int dWidth, endian_t eType)
{
    MemoryRegion *region = new MemoryRegion(&sysMachine, name, bytes, dWidth, eType);

    if (region != nullptr)
        regions.emplace(name, region);
    return region;
}

void MemoryManager::releaeRegion(cstr_t &name)
{
    regions.erase(name);
}

MemoryRegion *MemoryManager::findRegion(cstr_t &name)
{
    auto entry = regions.find(name);
    if (entry != regions.end())
        return entry->second;
    return nullptr;
}

// Memory share function call definitions

MemoryShare *MemoryManager::allocateShare(Device &owner, AddressType space, cstr_t &name,
    size_t bytes, int dWidth, endian_t eType)
{
    if (findShare(name) != nullptr)
        return nullptr;
    
    uint8_t *data = allocateMemory(owner, space, name, bytes, dWidth, eType);

    MemoryShare *share = new MemoryShare(data, name, bytes, dWidth, eType);
    if (share != nullptr)
        shares.emplace(name, share);
    return share;
}

MemoryShare *MemoryManager::findShare(cstr_t &name)
{
    auto entry = shares.find(name);
    if (entry != shares.end())
        return entry->second;
    return nullptr;
}

// Memory block function call definitions

MemoryBank *MemoryManager::findBank(cstr_t &name)
{
    auto entry = banks.find(name);
    if (entry != banks.end())
        return entry->second;
    return nullptr;
}
