
#include "emu/core.h"
#include "main/engine.h"

#include "printers/epson/externs.h"
#include "terminals/dec/externs.h"

const SystemDriver *SystemEngine::sysList[] =
{
    // Printer series
    &SYSTEM_NAME(mx80),
    &SYSTEM_NAME(mx100),

    // Terminal series
    &SYSTEM_NAME(vt100),

    // Terminator
    nullptr
};