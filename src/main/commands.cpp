// commands.cpp - system engine (commands) package
//
// Date:    May 2, 2023
// Author:  Tim Stark

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/memmgr.h"
#include "emu/dimem.h"
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

template <map::AddressType type>
SystemEngine::cmdStatus SystemEngine::cmdDumpm(UserConsole *user, args_t &args)
{
    using namespace map;

    offs_t sAddr, eAddr = -1ull;
    Device *dev = nullptr;

    if (!args.isEmpty())
    {
        char *strAddr;

        dev = findDevice(args.getArgument());
        if (dev == nullptr)
        {
            if (dialedSystem != nullptr)
                std::cout << fmt::format("{}: unknown device - aborted\n", args.getArgument());
            else
                std::cout << fmt::format("{}: Please dial system first - aborted\n", args.getArgument());
            return cmdOk;
        }

        args.getNext();
        sscanf(args.getArgument().c_str(), "%llx", &sAddr);
        if ((strAddr = strchr(args.getArgument().c_str(), '-')) != nullptr)
            sscanf(strAddr+1, "%llx", &eAddr);
        else
        {
            if (args.getSize() > 3)
            {
                args.getNext();
                sscanf(args.getArgument().c_str(), "%x", &eAddr);
                eAddr = sAddr + eAddr - 1;
            }
            else if (eAddr == -1)
                eAddr = sAddr + 0x140 - 1;
        }
    }

    diMemory *bus = nullptr;
    if (!dev->hasInterface(bus))
    {
        std::cout << fmt::format("{}: do not have bus interface - aborted\n",
            dev->getsDeviceName());
        return cmdOk;
    }

    AddressSpace  *space = bus->getAddressSpace(type);
    cAddressConfig *config = bus->getAddressConfig(type);
    if (space == nullptr)
    {
        std::cout << fmt::format("{}: {} address space is not available\n",
            dev->getsDeviceName(), map::asInfo[type]);
        return cmdOk;
    }

    int idx;
    char line[256], lasc[32];
    char *lptr, *pasc;
    uint64_t data;

    while (sAddr <= eAddr)
    {
        lptr = line;
        pasc = lasc;
        lptr += sprintf(lptr, "%0*llX: ", config->getAddrPrecision(), sAddr);
        for (idx = 0; (idx < 16) && (sAddr <= eAddr); idx++)
        {
            data = space->read8(sAddr++);
            lptr += sprintf(lptr, "%0*llX%c", config->getDataPrecision(),
                data, (idx == 7) ? '-' : ' ');
            *pasc++ = ((data >= 32) && (data < 127)) ? data : '.';
        }
        *pasc = '\0';
        *lptr = '\0';

        std::cout << fmt::format("{} |{:<15}|\n", line, lasc);
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
    { "dumpp",    &SystemEngine::cmdDumpm<map::asProgram>,  nullptr },
    { "dumpd",    &SystemEngine::cmdDumpm<map::asData>,     nullptr },
    { "dumpio",   &SystemEngine::cmdDumpm<map::asIOPort>,   nullptr },
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