// machine.h - machine engine package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

class UserConsole;

#include "emu/map/memmgr.h"
#include "emu/scheduler.h"
#include "emu/video.h"

class Machine
{
public:
    Machine(const SystemConfig &config, cstag_t &sysName);
    ~Machine();

    inline cstag_t getDeviceName() const { return sysDevice->getsDeviceName(); }
    inline Device *getSystemDevice() const { return sysDevice; }

    inline map::MemoryManager &getMemoryManager() { return memoryManager; }

    static Machine *create(UserConsole *user, const SystemDriver *driver, cstag_t &devName);

    // Function calls from system engine
    void start(UserConsole *user);
    void setConsole(UserConsole *user);
    
    void startAllDevices(UserConsole *user);

private:
    const SystemConfig &config;
    cstag_t sysName;
    Device *sysDevice = nullptr;

    map::MemoryManager memoryManager;
    Scheduler scheduler;
    VideoManager video;
};