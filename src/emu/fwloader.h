// fwloader.h - firmware loader package
//
// Author:  Tim Stark
// Date:    1/2/22

#pragma once

#include "emu/map/fw.h"

class FirmwareLoader
{
public:
    FirmwareLoader(Machine &sys, UserConsole &user);
    ~FirmwareLoader() = default;

    inline Machine &getSystemMachine() const { return sysMachine; }

protected:
    cfwEntry_t *first(Device &dev);
    cfwEntry_t *next(cfwEntry_t *entry);

    void processImageEntries(ctag_t *pkgName, cfwEntry_t *&entry, const Device &dev);
    void processRegionList();

    Machine &sysMachine;

    map::MemoryRegion *region = nullptr;
};