// er1400.cpp - GI ER100
//
// Author:  Tim Stark
// Date:    Jan 12, 2022

#include "emu/core.h"
#include "dev/chip/er1400.h"

er1400_Device::er1400_Device(const SystemConfig &config, const DeviceType &type, cstag_t &devName, Device *owner, uint64_t clock)
: Device(config, type, devName, owner, clock)
{ }

// ********

DEFINE_DEVICE_TYPE(ER1400, er1400_Device, "er1400", "ER1400 Serial EAROM (100x14)")

er1400_Device::er1400_Device(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock)
: er1400_Device(config, ER1400, devName, owner, clock)
{ }

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
            break;
        case 7: // accept data
            dataRegister = (dataRegister & 0x1FFF) << 1;
            dataRegister |= dataInput;
            break;
        }
    }
}

void er1400_Device::readData()
{

}

void er1400_Device::writeData()
{

}

void er1400_Device::eraseData()
{

}