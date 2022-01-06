// devfind.h - Device finder package
//
// Author:  Tim Stark
// Date:    12/10/2021

#include "emu/core.h"
#include "emu/map/memlib.h"
#include "emu/devfind.h"

ObjectFinder::ObjectFinder(Device &owner, ctag_t *name)
: base(owner), objName(name),
  fullObjectName(base.expandPathName(std::string(name)))
{
    owner.registerObject(this);
}

void ObjectFinder::setObjectName(ctag_t *name)
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

bool ObjectFinder::validate(bool found, bool required, ctag_t *name)
{
    if (required && objName == nullptr)
    {
        fmt::printf("%s: %s (unknown object name) is not defined as required\n",
            base.getsDeviceName(), name);
        return false;
    }
    else if (found)
    {
        fmt::printf("%s: %s %s '%s' now found and linked\n",
            base.getsDeviceName(), required ? "required" : "optional", name, objName);
        return true;
    }
    else
    {
        if (required)
            fmt::printf("%s: required %s '%s' not found\n", base.getsDeviceName(), name, objName);
        else if (objName != nullptr)
            fmt::printf("%s: optional %s '%s' not found\n", base.getsDeviceName(), name, objName);
        return !required;
    }
}