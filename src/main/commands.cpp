// commands.cpp - user commands
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/8/2021

#include "emu/core.h"
#include "emu/machine.h"
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

    // Create and dial system machine
    Machine *sysMachine = createMachine(user, devName, sysName);
    if (sysMachine != nullptr)
    {
        dial(sysMachine, user);
        fmt::printf("(%s): Dialed system '%s'\n",
            args[0], devName);
    }

    return cmdOk;
}

SystemEngine::cmdStatus SystemEngine::cmdDial(UserConsole *user, args_t &args)
{
    std::string devName = args.getNext();

    if (devName == "none")
    {
        dial(nullptr, user);
        fmt::printf("(%s): Dialed system released\n", args[0]);
    }
    else
    {
        Machine *sysMachine = findSystem(devName);
        if (sysMachine == nullptr)
        {
            fmt::printf("(%s): system '%s' not found - not dialed\n",
                args[0], devName);
            return cmdOk;
        }

        dial(sysMachine, user);
        fmt::printf("(%s): Dialed system '%s'\n",
            args[0], devName);
    }

    return cmdOk;
}

SystemEngine::cmdStatus SystemEngine::cmdStart(UserConsole *user, args_t &args)
{
    std::string devName = args.getNext();
    Machine *sysMachine = findSystem(devName);

    if (sysMachine == nullptr)
    {
        fmt::printf("%s: system not found\n", devName);
        return cmdOk;
    }

    // Start system machine (final initialization)
    sysMachine->start(user);

    return cmdOk;
}

SystemEngine::cmdStatus SystemEngine::cmdQuit(UserConsole *user, args_t &args)
{
    return cmdShutdown;
}

SystemEngine::command_t SystemEngine::mseCommands[] =
{
    { "create", &SystemEngine::cmdCreate, nullptr },
    { "dial",   &SystemEngine::cmdDial,   nullptr },
    { "exit",   &SystemEngine::cmdQuit,   nullptr },
    { "start",  &SystemEngine::cmdStart,  nullptr },
    { "quit",   &SystemEngine::cmdQuit,   nullptr },
    
    // Terminator
    nullptr
};