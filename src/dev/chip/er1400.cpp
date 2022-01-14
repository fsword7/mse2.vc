// er1400.cpp - GI ER100
//
// Author:  Tim Stark
// Date:    Jan 12, 2022

#include "emu/core.h"
#include "dev/chip/er1400.h"

er1400_Device::er1400_Device(const SystemConfig &config, const DeviceType &type, cstag_t &devName, Device *owner, uint64_t clock)
: Device(config, type, devName, owner, clock),
  diNonVolatileMemory(*this),
  defaultData(*this, DEVICE_SELF)
{ }

// ********

DEFINE_DEVICE_TYPE(ER1400, er1400_Device, "er1400", "ER1400 Serial EAROM (100x14)")

er1400_Device::er1400_Device(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: er1400_Device(config, ER1400, devName, owner, clock)
{ }

// ********

void er1400_Device::devStart()
{
    dataArray = std::make_unique<uint16_t[]>(100);
}

// ********

void er1400_Device::initnvr()
{
    if (defaultData.wasFound())
        // Load default data from region memory space
        std::copy_n(&defaultData[0], 100, &dataArray[0]);
    else
        // Erase all locations as initialization at once
        std::fill_n(&dataArray[0], 100, 0);
}

void er1400_Device::readnvr(emu::ioFile &file)
{
    file.read(reinterpret_cast<uint8_t *>(&dataArray[0]), 100 * sizeof(dataArray[0]));
}

void er1400_Device::writenvr(emu::ioFile &file)
{
    file.write(reinterpret_cast<uint8_t *>(&dataArray[0]), 100 * sizeof(dataArray[0]));
}

// ********

void er1400_Device::readData()
{
    int selected = 0;
    dataRegister = 0;
    for (int tens = 10; tens < 20; tens++)
    {
        if ((addressRegister >> tens) & 1)
        {
            for (int units = 0; units < 10; units++)
            {
                if ((addressRegister >> units) & 1)
                {
                    offs_t offset = 10 * (tens - 10) + units;
                    dataRegister |= dataArray[offset];
                    selected++;
                }
            }
        }
    }
}

void er1400_Device::writeData()
{
    int selected = 0;
    for (int tens = 10; tens < 20; tens++)
    {
        if ((addressRegister >> tens) & 1)
        {
            for (int units = 0; units < 10; units++)
            {
                if ((addressRegister >> units) & 1)
                {
                    offs_t offset = 10 * (tens - 10) + units;
                    if ((~dataArray[offset] & dataRegister) != 0)
                    {
                        dataArray[offset] |= dataRegister;
                    }
                    selected++;
                }
            }
        }
    }
}

void er1400_Device::eraseData()
{
    int selected = 0;
    for (int tens = 10; tens < 20; tens++)
    {
        if ((addressRegister >> tens) & 1)
        {
            for (int units = 0; units < 10; units++)
            {
                if ((addressRegister >> units) & 1)
                {
                    offs_t offset = 10 * (tens - 10) + units;
                    if (dataArray[offset] != 0)
                    {
                        dataArray[offset] = 0;
                    }
                    selected++;
                }
            }
        }
    }
}

// ********

void er1400_Device::write1c1(int state)
{
    if (bool(state) == ((codeInput >> 2) & 1))
        return;
    codeInput = (codeInput & 3) | (bool(state) << 2);
}

void er1400_Device::write1c2(int state)
{
    if (bool(state) == ((codeInput >> 1) & 1))
        return;
    codeInput = (codeInput & 5) | (bool(state) << 1);
}

void er1400_Device::write1c3(int state)
{
    if (bool(state) == ((codeInput >> 0) & 1))
        return;
    codeInput = (codeInput & 6) | bool(state);    
}

void er1400_Device::write1clock(int state)
{
    if (bool(state) == clockInput)
        return;
    clockInput = bool(state);

    if (!state)
    {

        switch(codeInput)
        {
        case 0: // standby
            break;
        case 1: // not used
        default:
            break;
        case 2: // erase
            eraseData();
            break;
        case 3: // accept address
            addressRegister = (addressRegister << 1) | dataInput;
            addressRegister &= 0xFFFFF;
            break;
        case 4: // read
            readData();
            break;
        case 5: // shift data out
            dataRegister = (dataRegister & 0x1FFF) << 1;
            break;
        case 6: // write
            writeData();
            break;
        case 7: // accept data
            dataRegister = (dataRegister & 0x1FFF) << 1;
            dataRegister |= dataInput;
            break;
        }
    }
}
