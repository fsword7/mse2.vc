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
        // allocate(user, bus);
    }
}
