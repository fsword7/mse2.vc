// dinvram.h - Non-Volatile Memory Device Interface package
//
// Author:  Tim Stark
// Date:    Jan 14, 2021

#pragma once

#include "emu/fileio.h"

class diNonVolatileMemory : public DeviceInterface
{
public:
    diNonVolatileMemory(Device *owner);
    virtual ~diNonVolatileMemory() = default;
    
    void reset()                 { initnvr(); };
    void load(emu::ioFile &file) { readnvr(file); }
    void save(emu::ioFile &file) { writenvr(file); };
    bool canSave()               { return canWrite(); }

protected:
    virtual void initnvr() {}
    virtual void readnvr(emu::ioFile &file) {}
    virtual void writenvr(emu::ioFile &file) {}
    virtual bool canWrite() { return true; }
};