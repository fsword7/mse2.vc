// fwloader.h - firmware loader package
//
// Author:  Tim Stark
// Date:    1/2/22

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/memmgr.h"
#include "main/user.h"
#include "emu/machine.h"
#include "emu/fwloader.h"

FirmwareLoader::FirmwareLoader(Machine &sys, UserConsole &user)
: sysMachine(sys)
{

}

cfwEntry_t*FirmwareLoader::first(Device &dev)
{
    cfwEntry_t *entry = dev.getFirmwareEntries();

    return !FWENTRY_ISEND(entry) ? entry : nullptr;
}

cfwEntry_t *FirmwareLoader::next(cfwEntry_t *entry)
{
    while (!FWENTRY_ISREGIONEND(entry))
        entry++;

    return !FWENTRY_ISEND(entry) ? entry : nullptr;
}

void FirmwareLoader::processImageEntries(ctag_t *pkgName, cfwEntry_t *&entry, const Device &dev)
{

}

void FirmwareLoader::processRegionList()
{
    ctag_t *pkgName;
    ctag_t *rgnName;
    uint32_t rgnLength;
    cfwEntry_t *entry;

    map::MemoryManager &bus = sysMachine.getMemoryManager();

    for (Device &dev : DeviceIterator(*sysMachine.getSystemDevice()))
    {
        pkgName = dev.getShortName();

        fmt::printf("%s: Initializing firmware entries for %s...\n",
            dev.getDeviceName(), pkgName);
        for (entry = first(dev); entry != nullptr; entry = next(entry))
        {
            if (FWENTRY_ISCONTAINER(entry))
            {
                pkgName = FW_GETNAME(entry);
                fmt::printf("%s: Package '%s' container\n",
                    dev.getDeviceName(), pkgName);
                continue;
            }

            rgnName = FWREGION_GETNAME(*entry);
            rgnLength = FWREGION_GETLENGTH(*entry);

            fmt::printf("%s: Processing firmwarw region '%s' length %d (%X) bytes\n",
                dev.getDeviceName(), rgnName, rgnLength, rgnLength);

            if (FWREGION_ISROMDATA(*entry))
            {
                uint8_t  dWidth = FWREGION_GETWIDTH(*entry);
                endian_t eType = FWREGION_ISBIGENDIAN(*entry) ? BigEndian : LittleEndian;
                uint8_t  fill = 0;

                region = bus.allocateRegion(dev, map::asProgram, rgnName, rgnLength, dWidth, eType);

                if (FWREGION_HASFILLVALUE(*entry))
                    fill = FWREGION_GETFILL(*entry);
                memset(region->getBase(), fill, region->getSize());

                processImageEntries(pkgName, entry, dev);
            }
        }
        fmt::printf("%s: End of firmware entries\n", dev.getDeviceName());
    }
}