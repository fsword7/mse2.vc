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

    // Bind callback function calls
    auto bindTXDHandler() { return txdHandler.bind(); }
    auto bindDTRHandler() { return dtrHandler.bind(); }
    auto bindRTSHandler() { return rtsHandler.bind(); }
    auto bindRXRHandler() { return rxrHandler.bind(); }
    auto bindTXRHandler() { return txrHandler.bind(); }
    auto bindTXEHandler() { return txeHandler.bind(); }
    auto bindSYDHandler() { return sydHandler.bind(); }
    
protected:
    i8251_Device(const SystemConfig &config, const DeviceType &type,
        cstag_t &devName, Device *owner, uint64_t clock);

private:
    write1cb_t txdHandler;
    write1cb_t dtrHandler;
    write1cb_t rtsHandler;
    write1cb_t rxrHandler;
    write1cb_t txrHandler;
    write1cb_t txeHandler;
    write1cb_t sydHandler;

};

DECLARE_DEVICE_TYPE(I8251, i8251_Device)