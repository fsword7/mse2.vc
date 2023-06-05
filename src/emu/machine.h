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
    Machine(cSystemConfig &config, cstr_t &sysName);
    ~Machine() = default;

    inline cstr_t getDeviceName() const     { return sysDevice->getsDeviceName(); }
    inline Device *getSystemDevice() const  { return sysDevice; }

    static Machine *create(UserConsole *user, cSystemDriver &driver, cstr_t &devName);

    void setConsole(UserConsole *user);
    void startAllDevices(UserConsole *user);
    void start(UserConsole *user);

private:
    cSystemConfig &config;
    cstr_t sysName;
    Device *sysDevice = nullptr;

    map::MemoryManager memoryManager;
    // Scheduler scheduler;
    // VideoManager video;
};