// memmgr.h - Memory Manager package
//
// Author:  Tim Stark
// Date:    May 11, 2023

class MemoryManager
{
public:
    MemoryManager(Machine &sys) : sysMachine(sys)
    { }
    ~MemoryManager() = default;

    inline Machine &getMachine() const      { return sysMachine; }

private:
    Machine &sysMachine;

};