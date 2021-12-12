// machine.h - machine engine package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

class UserConsole;

#include "emu/map/memmgr.h"

class Machine
{
public:
    Machine(const SystemConfig &config, cstag_t &sysName);
    ~Machine();

    cstag_t getDeviceName() const { return std::string(sysDevice->getDeviceName()); }
    
    static Machine *create(UserConsole *user, const SystemDriver *driver, cstag_t &devName);

private:
    const SystemConfig &config;
    cstag_t sysName;
    Device *sysDevice = nullptr;

    // MemoryManager memoryManager;
};