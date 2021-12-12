// memmgr.h - memory manager package
//
// Author:  Tim Stark
// Date:    12/10/2021

class Machine;
class diMemory;

namespace aspace
{
    class MemoryManager
    {
        public:
            MemoryManager(Machine &sys) : sysMachine(sys) {}
            ~MemoryManager() = default;

            void allocate(UserConsole *user, diMemory &bus);

        private:
            Machine &sysMachine;
    };
}