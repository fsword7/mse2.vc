// commands.cpp - user commands
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/8/2021

#include "emu/core.h"
#include "emu/machine.h"
#include "main/engine.h"
#include "main/user.h"

#include "emu/map/map.h"
#include "emu/dimem.h"

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

SystemEngine::cmdStatus SystemEngine::cmdDump(UserConsole *user, args_t &args)
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
                fmt::printf("%s: unknown device - aborted\n", args.getArgument());
            else
                fmt::printf("%s: Please dial system first - aborted\n", args.getArgument());
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
        fmt::printf("%s: do not have bus interface - aborted\n",
            dev->getDeviceName());
        return cmdOk;
    }
    AddressSpace  *space = bus->getAddressSpace();
    cAddressConfig *config = bus->getAddressConfig();

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

        fmt::printf("%s |%-15s|\n", line, lasc);
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
    { "dump",   &SystemEngine::cmdDump,   nullptr },
    { "exit",   &SystemEngine::cmdQuit,   nullptr },
    { "start",  &SystemEngine::cmdStart,  nullptr },
    { "quit",   &SystemEngine::cmdQuit,   nullptr },
    
    // Terminator
    nullptr
};