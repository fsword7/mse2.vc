// commands.cpp - system engine (commands) package
//
// Date:    May 2, 2023
// Author:  Tim Stark

#include "emu/core.h"
#include "main/engine.h"
#include "main/user.h"

SystemEngine::cmdStatus SystemEngine::cmdCreate(UserConsole *user, args_t &args)
{
    str_t devName = args.getNext();
    str_t sysName = args.getNext();

    if (findSystem(devName) != nullptr)
    {
        std::cout << fmt::format("{}: System already created.\n", devName);
        return cmdOk;
    }

    // Create and dial system machine
    Machine *sysMachine = createMachine(user, devName, sysName);
    if (sysMachine != nullptr)
    {
        dial(sysMachine, user);
        std::cout << fmt::format("({}): Dialed system '{}'\n",
            args[0], devName);
    }

    return cmdOk;
}

SystemEngine::cmdStatus SystemEngine::cmdQuit(UserConsole *user, args_t &args)
{
    return cmdShutdown;
}

SystemEngine::command_t SystemEngine::mseCommands[] =
{
    { "create",   &SystemEngine::cmdCreate,                 nullptr },
    // { "dial",     &SystemEngine::cmdDial,                   nullptr },
    // { "dumpp",    &SystemEngine::cmdDump<map::asProgram>,   nullptr },
    // { "dumpd",    &SystemEngine::cmdDump<map::asData>,      nullptr },
    // { "dumpio",   &SystemEngine::cmdDump<map::asIOPort>,    nullptr },
    // { "dumpm",    &SystemEngine::cmdDumpm,                  nullptr },
    // { "list",     &SystemEngine::cmdList,                   nullptr },
    // { "exit",     &SystemEngine::cmdQuit,                   nullptr },
    // { "start",    &SystemEngine::cmdStart,                  nullptr },
    // { "step",     &SystemEngine::cmdStep,                   nullptr },
    { "quit",     &SystemEngine::cmdQuit,                   nullptr },

    // Terminator
    nullptr
};