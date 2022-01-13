// diserial.h - Serial Device Interface
//
// Author:  Tim Stark
// Date:    Jan 12, 2022

#pragma once

class diSerial : public DeviceInterface
{
public:
    diSerial(Device *owner);
    virtual ~diSerial() = default;

protected:
    uint8_t parityTable[256];

};