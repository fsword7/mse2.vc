
#include "emu/core.h"
#include "main/engine.h"

#include "terminals/dec/externs.h"

const SystemDriver *SystemEngine::sysList[] =
{
    // Terminal series
    &SYSTEM_NAME(vt100),

    // Terminator
    nullptr
};