// diserial.h - Serial Device Interface
//
// Author:  Tim Stark
// Date:    Jan 12, 2022

#include "emu/core.h"
#include "emu/diserial.h"

diSerial::diSerial(Device *owner)
: DeviceInterface(owner, "serial")
{ 
    // Initialize serial parity table
    for (int data = 0; data < 256; data++)
    {
        int sum = 0;
        for (int bit = 0; bit < 8; bit++)
            if (data & (1 << bit))
                sum++;

        parityTable[data] = sum & 1;
    }
}