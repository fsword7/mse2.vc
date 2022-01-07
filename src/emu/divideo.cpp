// divideo.cpp - Video Device Interface package
//
// Author:  Tim Stark
// Date:    Dec 17, 2021

#include "emu/core.h"
#include "emu/divideo.h"

diVideo::diVideo(Device *owner)
: DeviceInterface(owner, "video")
{
}