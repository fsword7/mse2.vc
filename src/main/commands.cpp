// commands.cpp - user commands
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/8/2021

#include "emu/core.h"
#include "main/engine.h"
#include "main/user.h"

int cmdCreate(UserConsole *user, SystemEngine *engine, args_t &args)
{
    fmt::printf("Here is create command\n");
    return 0;
}

command_t mseCommands[] =
{
    { "create", &cmdCreate, nullptr },
    // Terminator
    nullptr
};