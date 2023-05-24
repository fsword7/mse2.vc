// didebug.h - Device Interface - Debug Interface package
//
// Author:  Tim Stark
// Date:    May 24, 2023

#pragma once

class diDebug : public DeviceInterface
{
public:
    diDebug(Device *owner);
    virtual ~diDebug() = default;

};