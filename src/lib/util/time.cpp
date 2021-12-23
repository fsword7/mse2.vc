// time.cpp - Attotime package
//
// Author:  Tim Stark
// Date:    12/23/21

#include "emu/core.h"
#include "lib/util/time.h"

using namespace emu::time;

Attotime const Attotime::zero = Attotime(0);
Attotime const Attotime::never = Attotime(ATTOTIME_MAX_SECONDS);