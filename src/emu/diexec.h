// diexec.h - Execution Device Interface package
//
// Author:  Tim Stark
// Date:    Dec 10, 2021

#pragma once

class diExecute : public DeviceInterface
{
public:
    diExecute(Device *owner);
    virtual ~diExecute() = default;

    virtual void step() { }
    virtual void executeRun() { }
    
private:
};