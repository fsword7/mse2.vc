// commands.cpp - system engine (commands) package
//
// Date:    May 2, 2023
// Author:  Tim Stark

#include "emu/core.h"
#include "emu/map/memmgr.h"
#include "emu/machine.h"
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

SystemEngine::cmdStatus SystemEngine::cmdDial(UserConsole *user, args_t &args)
{
    std::string devName = args.getNext();

    if (devName == "none")
    {
        dial(nullptr, user);
        std::cout << fmt::format("({}): Dialed system released\n", args[0]);
    }
    else
    {
        Machine *sysMachine = findSystem(devName);
        if (sysMachine == nullptr)
        {
            std::cout << fmt::format("({}): system '{}' not found - not dialed\n",
                args[0], devName);
            return cmdOk;
        }

        dial(sysMachine, user);
        std::cout << fmt::format("({}): Dialed system '{}'\n",
            args[0], devName);
    }

    return cmdOk;
}

SystemEngine::cmdStatus SystemEngine::cmdListr(UserConsole *user, args_t &args)
{
    Machine *sysMachine = dialedMachine;
    map::MemoryManager &mgr = sysMachine->getMemoryManager();

    // Make ensure that system is dialed
    if (sysMachine == nullptr)
    {
        std::cout << fmt::format("{}: Plese dial system first\n", args[0]);
        return cmdOk;
    }

    map::RegionList list = mgr.getRegionList();
    for (auto iter : list)
    {
        map::MemoryRegion *region = iter.second;

        std::cout << fmt::format("{}\n", region->getsName());
    }

    return cmdOk;
}

SystemEngine::cmdStatus SystemEngine::cmdStart(UserConsole *user, args_t &args)
{
    std::string devName = args.getNext();
    Machine *sysMachine = findSystem(devName);

    if (sysMachine == nullptr)
    {
        std::cout << fmt::format("{}: system not found\n", devName);
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
    { "create",   &SystemEngine::cmdCreate,                 nullptr },
    { "dial",     &SystemEngine::cmdDial,                   nullptr },
    // { "dumpp",    &SystemEngine::cmdDump<map::asProgram>,   nullptr },
    // { "dumpd",    &SystemEngine::cmdDump<map::asData>,      nullptr },
    // { "dumpio",   &SystemEngine::cmdDump<map::asIOPort>,    nullptr },
    // { "dumpm",    &SystemEngine::cmdDumpm,                  nullptr },
    // { "list",     &SystemEngine::cmdList,                   nullptr },
    { "listr",    &SystemEngine::cmdListr,                  nullptr },
    // { "exit",     &SystemEngine::cmdQuit,                   nullptr },
    { "start",    &SystemEngine::cmdStart,                  nullptr },
    // { "step",     &SystemEngine::cmdStep,                   nullptr },
    { "quit",     &SystemEngine::cmdQuit,                   nullptr },

    // Terminator
    nullptr
};