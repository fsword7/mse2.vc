// diexec.h - Device Interface - Execution
//
// Author:  Tim Stark
// Date:    12/10/2021

#pragma once

class diExecute : public DeviceInterface
{
    public:
        diExecute(Device *owner);
        virtual ~diExecute() = default;

     private:
};