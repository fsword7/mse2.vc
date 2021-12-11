// devfind.h - Device finder package
//
// Author:  Tim Stark
// Date:    12/10/2021

#include "emu/core.h"
#include "emu/devfind.h"

ObjectFinder::ObjectFinder(Device &owner, ctag_t *name)
: base(owner), objName(name)
{
    owner.registerObject(this);
}

void ObjectFinder::setObjectName(ctag_t *name)
{
    assert(!isResolved);
    objName = name;
}