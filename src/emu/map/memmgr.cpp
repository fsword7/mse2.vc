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