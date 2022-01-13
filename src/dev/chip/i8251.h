// i8251.cpp - Intel 8251 USART (Universal Synchronous/Asynchronous Receiver Transmitter)
//
// Author:  Tim Stark
// Date:    Jan 12, 2022

#pragma once

#include "emu/diserial.h"
#include "emu/devcb.h"

class i8251_Device : public Device, public diSerial
{
public:
    i8251_Device(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);

    uint8_t read8io(offs_t offset) { return 0; }
    void write8io(offs_t offset, uint8_t data) { }

protected:
    i8251_Device(const SystemConfig &config, const DeviceType &type,
        cstag_t &devName, Device *owner, uint64_t clock);

};

DECLARE_DEVICE_TYPE(I8251, i8251_Device)