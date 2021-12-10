// commands.cpp - user commands
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/8/2021

#include "emu/core.h"
#include "main/engine.h"
#include "main/user.h"

int cmdCreate(UserConsole *user, SystemEngine *engine, args_t &args)
{
    std::string devName = args.getNext();
    std::string sysName = args.getNext();

    if (engine->findSystem(devName) != nullptr)
    {
        fmt::printf("%s: System already created.\n", devName);
        return 0;
    }

    return engine->createMachine(user, devName, sysName);
}

int cmdQuit(UserConsole *user, SystemEngine *engine, args_t &args)
{
    return 1;
}

command_t mseCommands[] =
{
    { "create", &cmdCreate, nullptr },
    { "exit",   &cmdQuit,   nullptr },
    { "quit",   &cmdQuit,   nullptr },
    
    // Terminator
    nullptr
};