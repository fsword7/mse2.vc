// systems.cpp - Systems database package
//
// Date:    May 11, 2023
// Author:  Tim Stark

#include "emu/core.h"
#include "main/engine.h"

#include "systems/dec/externs.h"

const SystemDriver *SystemEngine::sysList[] =
{
    // Computer lists
    &SYSTEM_NAME(mv3900),
    &SYSTEM_NAME(vs3900),

    // ALphaStation/AlphaServer series
    &SYSTEM_NAME(as200),
    &SYSTEM_NAME(as400),
    &SYSTEM_NAME(es40),

    // Terminator
    nullptr
};