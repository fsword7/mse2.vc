// er1400.h - GI ER1400 - Serial Electronically Alterable Read-Only Memory
//
// Author:  Tim Stark
// Date:    Jan 13, 2022

#pragma once

#include "emu/dinvram.h"

class er1400_Device : public Device, public diNonVolatileMemory
{
public:
    er1400_Device(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);

    int read1d()             { return dataInput | dataOutput; }
    void write1d(int state)  { dataInput = bool(state); }

    void write1c1(int state);
    void write1c2(int state);
    void write1c3(int state);
    void write1clock(int state);

    // Virtual device function calls
    void devStart() override;

    // Non-volatile memory function calls
    void initnvr() override;
    void readnvr(emu::ioFile &file) override;
    void writenvr(emu::ioFile &file) override;

protected:
    er1400_Device(const SystemConfig &config, const DeviceType &type,
        cstag_t &devName, Device *owner, uint64_t clock);

    void readData();
    void writeData();
    void eraseData();
    
private:
    // serial memory space
    OptionalRegionPointer<uint16_t> defaultData;
    std::unique_ptr<uint16_t[]>     dataArray;

    bool dataInput;
    bool dataOutput;
    bool clockInput;
    uint8_t codeInput;

    uint16_t dataRegister;
    uint32_t addressRegister;
};

DECLARE_DEVICE_TYPE(ER1400, er1400_Device)