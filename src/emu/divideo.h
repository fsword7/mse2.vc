// divideo.h - Video Device Interface package
//
// Author:  Tim Stark
// Date:    Dec 17, 2021

#pragma once

#include "emu/screen.h"

class diVideo : public DeviceInterface
{
public:
    diVideo(Device *owner);
    virtual ~diVideo() = default;

    void setScreenName(ctag_t *name) { screenName = name; }
    
protected:
    bool screenRequired = false;
    Device *screenBase = nullptr;
    ctag_t *screenName = nullptr;

    // Screen *screen = nulllptr;
};