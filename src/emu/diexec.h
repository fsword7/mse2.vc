// diexec.h - Device Interface - Execution Interface package
//
// Author:  Tim Stark
// Date:    May 24, 2023

#pragma once

class diExecute : public DeviceInterface
{
public:
    diExecute(Device *owner);
    virtual ~diExecute() = default;

};