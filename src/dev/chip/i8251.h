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
    
    // register function calls
    uint8_t read8d()           { return 0; }
    void write8d(uint8_t data) {}
    uint8_t read8s()           { return 0; }
    void write8c(uint8_t data) {}

    virtual uint8_t read8(offs_t offset) { return 0; };
    virtual void write8(offs_t offset)   {}

    // signal functon calls
    void write1rxd(int state) {}
    void write1cts(int state) {}
    void write1dsr(int state) {}
    void write1txc(int state) {}
    void write1rxc(int state) {}
    void write1syn(int state) {}
    int read1txr() { return 0; }

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