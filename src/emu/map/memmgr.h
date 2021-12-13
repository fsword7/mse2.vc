// memmgr.h - memory manager package
//
// Author:  Tim Stark
// Date:    12/10/2021

#pragma once

class Machine;
class diMemory;
class UserConsole;

namespace map
{
    class MemoryManager
    {
        public:
            MemoryManager(Machine &sys) : sysMachine(sys) {}
            ~MemoryManager() = default;

            void init(UserConsole *user);
            void allocate(UserConsole *user, diMemory &bus);

        private:
            Machine &sysMachine;
    };
}