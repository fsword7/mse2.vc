// fwloader.h - firmware loader package
//
// Author:  Tim Stark
// Date:    1/2/22

#pragma once

#include "emu/map/fw.h"
#include "emu/fileio.h"

class FirmwareLoader
{
public:
    FirmwareLoader(Machine &sys, UserConsole &user);
    ~FirmwareLoader() = default;

    inline Machine &getSystemMachine() const { return sysMachine; }

protected:
    cfwEntry_t *first(Device &dev);
    cfwEntry_t *next(cfwEntry_t *entry);

    emu::ioFile *processImageFile(fs::path pname, cfwEntry_t *entry);
    void openImageFile(fs::path pkgName, cfwEntry_t *entry);
    void closeImageFile();
    int readImageData(uint8_t *buffer, int length, cfwEntry_t *entry);
    bool loadImageData(cfwEntry_t *parent, cfwEntry_t *entry);

    void processImageEntries(ctag_t *pkgName, cfwEntry_t *&entry, const Device &dev);
    void processRegionList();

    Machine &sysMachine;

    emu::ioFile *imageFile = nullptr;
    map::MemoryRegion *region = nullptr;
};