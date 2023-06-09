// machine.h - virtual machine package
//
// Date:    Apr 30, 2023
// Author:  Tim Stark
\
#pragma once

#include "emu/map/memmgr.h"

class Machine // : public sysDevice
{
public:
    Machine(UserConsole *user, cSystemDriver &driver, cstr_t &sysName);
    ~Machine() = default;

    inline cstr_t getDeviceName() const     { return sysDevice->getsDeviceName(); }
    inline Device *getSystemDevice() const  { return sysDevice; }
    inline map::MemoryManager &getMemoryManager() { return memoryManager; }

    void setConsole(UserConsole *user);
    void startAllDevices(UserConsole *user);
    void start(UserConsole *user);

private:
    SystemConfig config;
    cstr_t sysName;
    Device *sysDevice = nullptr;

    UserConsole *user = nullptr;

    map::MemoryManager memoryManager;
    // Scheduler scheduler;
    // VideoManager video;
};