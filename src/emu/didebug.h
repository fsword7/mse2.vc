// didebug.h - Debugging/Disassembler Device Interface package
//
// Author:  Tim Stark
// Date:    Jan 7, 2022

#pragma once

class diDebug : public DeviceInterface
{
public:
    diDebug(Device *owner);
    ~diDebug() = default;

    virtual int list(offs_t vAddr) { return 0; };

private:
};