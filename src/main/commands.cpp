// commands.cpp - user commands
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/8/2021

#include "emu/core.h"
#include "main/engine.h"
#include "main/user.h"

SystemEngine::cmdStatus SystemEngine::cmdCreate(UserConsole *user, args_t &args)
{
    std::string devName = args.getNext();
    std::string sysName = args.getNext();

    if (findSystem(devName) != nullptr)
    {
        fmt::printf("%s: System already created.\n", devName);
        return cmdOk;
    }

    createMachine(user, devName, sysName);
    return cmdOk;
}

SystemEngine::cmdStatus SystemEngine::cmdQuit(UserConsole *user, args_t &args)
{
    return cmdShutdown;
}

SystemEngine::command_t SystemEngine::mseCommands[] =
{
    { "create", &SystemEngine::cmdCreate, nullptr },
    { "exit",   &SystemEngine::cmdQuit,   nullptr },
    { "quit",   &SystemEngine::cmdQuit,   nullptr },
    
    // Terminator
    nullptr
};