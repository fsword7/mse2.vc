// fwloader.cpp - Firmware Loader package
//
// Author:  Tim Stark
// Date:    Jun 17, 2023

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/memmgr.h"
#include "main/user.h"
#include "emu/machine.h"
#include "emu/fwloader.h"

FirmwareLoader::FirmwareLoader(Machine &sys, UserConsole &user)
: sysMachine(sys)
{
    processRegionList();
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

emu::ioFile *FirmwareLoader::processImageFile(fs::path pathName, cfwEntry_t *entry)
{
    fs::path fwPath = "fw";
    auto imageFile = new emu::ioFile(0);

    fs::path fileName = fwPath / pathName / FW_GETNAME(entry);

    if (!imageFile->open(fileName))
    {
        delete imageFile;
        imageFile = nullptr;
    }

    return imageFile;
}

void FirmwareLoader::openImageFile(fs::path pkgName, cfwEntry_t *entry)
{
    imageFile = processImageFile(pkgName, entry);
}

void FirmwareLoader::closeImageFile()
{
    imageFile->close();
    delete imageFile;
    imageFile = nullptr;
}

int FirmwareLoader::readImageData(uint8_t *buffer, int length, cfwEntry_t *entry)
{
    int actual = 0;

    if (imageFile != nullptr)
        actual = imageFile->read(buffer, length);
    return actual;
}

bool FirmwareLoader::loadImageData(cfwEntry_t *parent, cfwEntry_t *entry)
{
    int offset = FW_GETOFFSET(entry);
    int length = FW_GETLENGTH(entry);
    // int skip = FWIMAGE_GETSKIP(entry);
    // int dShift = FWIMAGE_GETBITSHIFT(entry);
    // int dMask = ((1 << FWIMAGE_GETBITWIDTH(entry)));
    // int gSize = FWIMAGE_GETGROUPSIZE(entry);
    // int reversed = FWIMAGE_ISREVERSED(entry);
    // int nGroups = (length + gSize - 1) / gSize;

    uint8_t *base = region->getBase() + offset;
    int actual;

    std::cout << fmt::format("{}: Reading image data: off={:X} len={:X}\n", FW_GETNAME(entry), offset, length);
    actual = readImageData(base, length, entry);
    if (actual != length)
    {
        std::cout << fmt::format("{}: Got {} ({:X}) bytes - expected {} ({:X}) bytes\n",
            FW_GETNAME(entry), actual, actual, length, length);
        return false;
    }
    return true;
}

void FirmwareLoader::processImageEntries(cchar_t *pkgName, cfwEntry_t *&entry, const Device &dev)
{
    cfwEntry_t *parent = entry++;

    while (!FWENTRY_ISREGIONEND(entry))
    {
        if (FWENTRY_ISFILE(entry))
        {
            cfwEntry_t *base = entry;
            int imageLength = 0;

            std::cout << fmt::format("{}: Loading image file '{}'...\n",
                dev.getsDeviceName(), FW_GETNAME(entry));
            openImageFile(pkgName, entry);

            if (imageFile != nullptr)
            {
                loadImageData(parent, entry);
                closeImageFile();
            }
            entry++;
        }
        else
            entry++;
    }
}

void FirmwareLoader::processRegionList()
{
    cchar_t *pkgName;
    cchar_t *rgnName;
    uint32_t rgnLength;
    cfwEntry_t *entry;

    map::MemoryManager &bus = sysMachine.getMemoryManager();

    for (Device &dev : DeviceIterator(*sysMachine.getSystemDevice()))
    {
        pkgName = dev.getShortName();

        std::cout << fmt::format("{}: Initializing firmware entries for {}...\n",
            dev.getsDeviceName(), pkgName);
        for (entry = first(dev); entry != nullptr; entry = next(entry))
        {
            if (FWENTRY_ISCONTAINER(entry))
            {
                pkgName = FW_GETNAME(entry);
                std::cout << fmt::format("{}: Package '{}' container\n",
                    dev.getsDeviceName(), pkgName);
                continue;
            }

            rgnName = FWREGION_GETNAME(*entry);
            rgnLength = FWREGION_GETLENGTH(*entry);

            cstr_t fullName = dev.expandPathName(str_t(rgnName));

            std::cout << fmt::format("{}: Processing firmware region '{}' length {} ({:X}) bytes\n",
                dev.getsDeviceName(), fullName, rgnLength, rgnLength);

            if (FWREGION_ISROMDATA(*entry))
            {
                uint8_t  dWidth = FWREGION_GETWIDTH(*entry);
                endian_t eType = FWREGION_ISBIGENDIAN(*entry) ? BigEndian : LittleEndian;
                uint8_t  fill = 0;

                region = bus.allocateRegion(dev, map::asProgram, fullName.c_str(), rgnLength, dWidth, eType);

                if (FWREGION_HASFILLVALUE(*entry))
                    fill = FWREGION_GETFILL(*entry);
                std::fill_n(region->getBase(), region->getSize(), fill);

                processImageEntries(pkgName, entry, dev);
            }
        }
        std::cout << fmt::format("{}: End of firmware entries\n", dev.getsDeviceName());
    }
}