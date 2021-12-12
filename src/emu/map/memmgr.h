// memmgr.h - memory manager package
//
// Author:  Tim Stark
// Date:    12/10/2021

class Machine;

namespace aspace
{
    class MemoryManager
    {
        public:
            MemoryManager(Machine &sys) : sysMachine(sys) {}
            ~MemoryManager() = default;

        private:
            Machine &sysMachine;
    };
}