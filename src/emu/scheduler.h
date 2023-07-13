// scheduler.h - Single-thread Scheduler package
//
// Auhor:   Tim Stark
// Date:    Jul 13, 2023

#pragma once

typedef NamedDelegate<void (void *, int64_t)> TimerDelegate_t;

class Timer
{
    friend class Scheduler;

public:
    Timer() = default;
    // Timer(Device &device, TimerDeviceID_t id, void *ptr, bool flag);
    Timer(Machine &system, TimerDelegate_t cb, void *ptr, bool flag);
    ~Timer() = default;

};

class Scheduler
{
public:
    Scheduler() = default;
    ~Scheduler() = default;
};