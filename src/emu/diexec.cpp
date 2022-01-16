// diexec.cpp - Execution Device Interface package
//
// Author:  Tim Stark
// Date:    Dec 10, 2021

#include "emu/core.h"
#include "emu/diexec.h"

diExecute::diExecute(Device *owner)
: DeviceInterface(owner, "execute")
{ }

void diExecute::diUpdateClock()
{
    cyclePerSecond = executeClockToCycle(getOwningDevice()->getClock());
    cycleDuration  = HZ_TO_ATTOSECONDS(cyclePerSecond);
}

attotime_t diExecute::getLocalTime() const
{
    if (isExecuting())
    {
        assert(cycleRunning >= *cycleCounter);
        int cycles = cycleRunning - *cycleCounter;
        return localTime + (cycles * cycleDuration);
    }
    return localTime;
}

void diExecute::eatCycles(int64_t cycles)
{
    if (!isExecuting() || cycleCounter == nullptr)
        return;

    if (cycles < 0 || cycles > *cycleCounter)
        *cycleCounter = 0;
    else
        *cycleCounter -= cycles;
}