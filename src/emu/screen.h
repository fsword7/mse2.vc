// screen.h - screen device package
//
// Author:  Tim Stark
// Date:    12/23/21

#pragma once

class ScreenDevice : public Device
{

};

DECLARE_DEVICE_TYPE(Screen, ScreenDevice);

using screen_t = ScreenDevice;