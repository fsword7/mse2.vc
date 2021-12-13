// memmgr.cpp - memory manager package
//
// Author:  Tim Stark
// Date:    12/10/2021

#include "emu/core.h"
#include "emu/machine.h"
#include "emu/dimem.h"

using namespace map;

void MemoryManager::init(UserConsole *user)
{
    fmt::printf("%s: Memory manager initialization\n",
        sysMachine.getDeviceName());

    std::vector<diMemory *> memories;

    memories.clear();
    for (diMemory &bus : InterfaceIterator<diMemory>(*sysMachine.getSystemDevice()))
    {
        memories.push_back(&bus);
        allocate(user, bus);
    }

    for (auto const bus : memories)
        bus->prepare(user);

    for (auto const bus : memories)
        bus->populate(user);   
}

// Memory block function call definitions

uint8_t *MemoryManager::allocateMemory(Device &owner, AddressType space, cstag_t &name,
    size_t bytes, int dWidth, endian_t eType)
{
    MemoryBlock *block = new MemoryBlock(name, bytes, dWidth, eType);

    if (block != nullptr)
        blocks.push_back(block);
    return block->getData();
}


// Memory region function call definitions

MemoryRegion *MemoryManager::allocateRegion(Device &owner, AddressType space, cstag_t &name,
    size_t bytes, int dWidth, endian_t eType)
{
    MemoryRegion *region = new MemoryRegion(&sysMachine, name, bytes, dWidth, eType);

    if (region != nullptr)
        regions.emplace(name, region);
    return region;
}

void MemoryManager::releaeRegion(cstag_t &name)
{
    regions.erase(name);
}

MemoryRegion *MemoryManager::findRegion(cstag_t &name)
{
    auto entry = regions.find(name);
    if (entry != regions.end())
        return entry->second;
    return nullptr;
}

// Memory share function call definitions

MemoryShare *MemoryManager::allocateShare(Device &owner, AddressType space, cstag_t &name,
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

MemoryShare *MemoryManager::findShare(cstag_t &name)
{
    auto entry = shares.find(name);
    if (entry != shares.end())
        return entry->second;
    return nullptr;
}

// Memory block function call definitions

MemoryBank *MemoryManager::findBank(cstag_t &name)
{
    auto entry = banks.find(name);
    if (entry != banks.end())
        return entry->second;
    return nullptr;
}
