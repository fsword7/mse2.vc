// diexec.h - Execution Device Interface package
//
// Author:  Tim Stark
// Date:    Dec 10, 2021

#pragma once

class diExecute : public DeviceInterface
{
public:
    diExecute(Device *owner);
    virtual ~diExecute() = default;

    inline bool isExecuting() const { return true; }

    virtual void step() { }

    void eatCycles(int64_t cycles);

    attotime_t getLocalTime() const;

    attoseconds_t getMinQuantum() const { return cycleDuration * executeGetMinCycles(); }

    // Virtual device interface function calls
    void diUpdateClock() override;

protected:
    // Virtual execute function calls for scheduler
    virtual uint64_t executeClockToCycle(uint64_t clocks) const { return clocks; }
    virtual uint64_t executeCycleToClock(uint64_t cycles) const { return cycles; }
    virtual uint64_t executeGetMinCycles() const { return 1; }
    virtual uint64_t executeGetMaxCycles() const { return 1; }
    virtual void executeRun() { }

    virtual int executeGetInputLines() const { return 0; }

private:
    // Scheduer parameters
    // Scheduler *schedular = nullptr;

    // Timing declartions
    uint64_t      cyclePerSecond = 0; // cycles per second (clock rate)
    attoseconds_t cycleDuration = 0; // cycle duration (in attoseconds)

    attotime_t  localTime;

    // Cycle countdown for scheduler
    int64_t     cycleTotal = 0;
    int64_t     cycleRunning = 0;
    int64_t     cycleStolen = 0;
    int64_t     *cycleCounter = nullptr;

    // Suspend state 
    uint64_t    suspend = 0;
    uint64_t    nextSuspend = 0;
    uint64_t    eatingCycles = 0;
    uint64_t    nextEatingCycles = 0;
};