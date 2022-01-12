// rs232.h - RS232 communication package
//
// Author:  Tim Stark
// Date:    Jan 12, 2022

#pragma once


// class rs232_portInterface : public DeviceInterface
// {

// };

class rs232_portDevice : public Device
{
public:
    rs232_portDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);

    rs232_portDevice(const SystemConfig &config, const DeviceType &type,
        cstag_t &devName, Device *owner, uint64_t clock);
    virtual ~rs232_portDevice() = default;

    void write1txd(int state)   { };    // DB25 pin 2
    void write1rts(int state)   { };    // DB25 pin 4
    void write1dtr(int state)   { };    // DB25 pin 20
    void writw1spds(int state)  { };    // DB25 pin 23
    void write1etc(int state)   { };    // DB25 pin 24

    int read1rxd() { return rxd; }  // DB25 pin 3
    int read1dcd() { return dcd; }  // DB25 pin 8
    int read1dsr() { return dsr; }  // DB25 pin 6
    int read1ri()  { return ri; }   // DB25 pin 22
    int read1si()  { return si; }
    int read1cts() { return cts; }  // DB25 pin 5
    int read1rxc() { return rxc; }  // DB25 pin 17
    int read1txc() { return txc; }  // DB25 pin 15

protected:
    // DB25 signals list
    int rxd = 0;
    int dcd = 0;
    int dsr = 0;
    int ri  = 0;
    int si  = 0;
    int cts = 0;
    int rxc = 0;
    int txc = 0;

    // DB25 signal handlers
    write1cb_t rxdHandler;
    write1cb_t dcdHandler;
    write1cb_t dsrHandler;
    write1cb_t riHandler;
    write1cb_t siHandler;
    write1cb_t ctsHandler;
    write1cb_t rxcHandler;
    write1cb_t txcHandler;
};

DECLARE_DEVICE_TYPE(RS232_PORT, rs232_portDevice);