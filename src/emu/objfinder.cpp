// objfinder.cpp - Object/Device Finder Package
//
// Author:  Tim Stark
// Date:    Jun 6, 2023

#include "emu/core.h"
#include "emu/map/memlib.h"
#include "emu/objfinder.h"

ObjectFinder::ObjectFinder(Device &owner, cchar_t *name)
: base(owner), objName(name),
  fullObjectName(base.expandPathName(std::string(name)))
{
    std::cout << fmt::format("{}: Set device finder name: {}\n", owner.getsDeviceName(), objName);
    owner.registerObject(this);
}

void ObjectFinder::setObjectName(cchar_t *name)
{
    assert(!isResolved);
    objName = name;
}

void *ObjectFinder::findMemoryRegion(uint8_t width, size_t &size, bool required) const
{
    map::MemoryRegion *region = base.findMemoryRegion(objName);

    if (region == nullptr)
    {
        size = 0;
        return nullptr;
    }

    // memory region had been found..
    size = region->getSize() / width;
    return region->getBase();
}

void *ObjectFinder::findMemoryShared(uint8_t width, size_t &size, bool required) const
{
    map::MemoryShare *share = base.findMemoryShare(objName);

    if (share == nullptr)
    {
        size = 0;
        return nullptr;
    }

    // memory region had been found..
    size = share->getBytes() / width;
    return share->getData();
}

bool ObjectFinder::validate(bool found, bool required, cchar_t *name)
{
    if (required && objName == nullptr)
    {
        std::cout << fmt::format("{}: {} (unknown object name) is not defined as required\n",
            base.getsDeviceName(), name);
        return false;
    }
    else if (found)
    {
        std::cout << fmt::format("{}: {} {} '{}' now found and linked\n",
            base.getsDeviceName(), required ? "required" : "optional", name, objName);
        return true;
    }
    else
    {
        if (required)
            std::cout << fmt::format("{}: required {} '{}' not found\n", base.getsDeviceName(), name, objName);
        else if (objName != nullptr)
            std::cout << fmt::format("{}: optional {} '{}' not found\n", base.getsDeviceName(), name, objName);
        return !required;
    }
}