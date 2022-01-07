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
#include "emu/didebug.h"

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

SystemEngine::cmdStatus SystemEngine::cmdDumpOld(UserConsole *user, args_t &args)
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

SystemEngine::cmdStatus SystemEngine::cmdDumpm(UserConsole *user, args_t &args)
{
    using namespace map;

    offs_t sAddr, eAddr = -1ull;
    Machine *sysMachine = dialedMachine;
    MemoryManager &mgr = sysMachine->getMemoryManager();
    std::string name = args.getNext();
    const uint8_t *base = nullptr;

    // Make ensure that system is dialed
    if (sysMachine == nullptr)
    {
        fmt::printf("%s: Plese dial system first\n", args[0]);
        return cmdOk;
    }

    // find named region/shared memory
    {
        map::MemoryRegion *region;
        map::MemoryShare *share;

        if (region = mgr.findRegion(name))
        {
            base  = region->getBase();
            sAddr = 0;
            eAddr = region->getSize() - 1;
        }
        else if (share = mgr.findShare(name))
        {
            base  = share->getData();
            sAddr = 0;
            eAddr = share->getBytes() - 1;
        }
        else
        {
            fmt::printf("%s: Can't find %s region space - aborted.\n", args[0], name);
            return cmdOk;
        }
    }


    // if (!args.isEmpty())
    // {
    //     char *strAddr;

    //     dev = findDevice(args.getArgument());
    //     if (dev == nullptr)
    //     {
    //         if (dialedSystem != nullptr)
    //             fmt::printf("%s: unknown device - aborted\n", args.getArgument());
    //         else
    //             fmt::printf("%s: Please dial system first - aborted\n", args.getArgument());
    //         return cmdOk;
    //     }

    //     args.getNext();
    //     sscanf(args.getArgument().c_str(), "%llx", &sAddr);
    //     if ((strAddr = strchr(args.getArgument().c_str(), '-')) != nullptr)
    //         sscanf(strAddr+1, "%llx", &eAddr);
    //     else
    //     {
    //         if (args.getSize() > 3)
    //         {
    //             args.getNext();
    //             sscanf(args.getArgument().c_str(), "%x", &eAddr);
    //             eAddr = sAddr + eAddr - 1;
    //         }
    //         else if (eAddr == -1)
    //             eAddr = sAddr + 0x140 - 1;
    //     }
    // }

    // diMemory *bus = nullptr;
    // if (!dev->hasInterface(bus))
    // {
    //     fmt::printf("%s: do not have bus interface - aborted\n",
    //         dev->getDeviceName());
    //     return cmdOk;
    // }
    // AddressSpace  *space = bus->getAddressSpace();
    // cAddressConfig *config = bus->getAddressConfig();

    int idx;
    char line[256], lasc[32];
    char *lptr, *pasc;
    uint64_t data;

    while (sAddr <= eAddr)
    {
        lptr = line;
        pasc = lasc;
        lptr += sprintf(lptr, "%08llX: ", /* config->getAddrPrecision(), */ sAddr);
        for (idx = 0; (idx < 16) && (sAddr <= eAddr); idx++)
        {
            data = base[sAddr++];
            lptr += sprintf(lptr, "%02X%c", /* config->getDataPrecision(), */
                data, (idx == 7) ? '-' : ' ');
            *pasc++ = ((data >= 32) && (data < 127)) ? data : '.';
        }
        *pasc = '\0';
        *lptr = '\0';

        fmt::printf("%s |%-15s|\n", line, lasc);
    }

    return cmdOk;
}

template <map::AddressType type>
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

    AddressSpace  *space = bus->getAddressSpace(type);
    cAddressConfig *config = bus->getAddressConfig(type);
    if (space == nullptr)
    {
        fmt::printf("%s: %s address space is not available\n",
            dev->getDeviceName(), map::asInfo[type]);
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

        fmt::printf("%s |%-15s|\n", line, lasc);
    }

    return cmdOk;
}

SystemEngine::cmdStatus SystemEngine::cmdList(UserConsole *user, args_t &args)
{
	Device *dev = nullptr;
	offs_t sAddr, eAddr = -1ull;
	int    count = 20;

	// if (!args.empty())
	{
		char *strAddr;

		dev = findDevice(user, args.getArgument());
		if (dev == nullptr) {
			fmt::printf("%s: unknown device\n", args.getArgument());
			return cmdOk;
		}

		args.getNext();
		sscanf(args.getArgument().c_str(), "%llx", &sAddr);
		if ((strAddr = strchr(args.getArgument().c_str(), '-')) != nullptr)
		{
			sscanf(strAddr+1, "%llx", &eAddr);
			count = (eAddr - sAddr) / 4;
		}
		else if (args.getSize() > 3)
		{
			args.getNext();
			sscanf(args.getArgument().c_str(), "%d", &count);
		}
	} 
    // else
    //     sAddr = user->getLastAddress(dev);

	diMemory *bus;
	diDebug *debug;
	if (!dev->hasInterface(bus))
	{
		fmt::printf("%s: do not have external bus interface\n", dev->getsDeviceName());
		return cmdOk;
	}
	map::AddressSpace *space = bus->getAddressSpace();

	if (!dev->hasInterface(debug))
	{
		fmt::printf("%s: do not have debug tools\n", dev->getsDeviceName());
		return cmdOk;
	}

	uint64_t addr = sAddr;
	uint32_t opCode;
	for (int idx = 0; idx < count; idx++)
	{
//		opCode = space->read32(addr);
//		fmt::printf("%llX  %08X\n", addr, opCode);
//
//		// next PC addrees
//		addr += 4;
		addr += debug->list(addr);
	}

	// Save device and current address for more output
	// user->setLastAddress(dev, addr);
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
    { "create",   &SystemEngine::cmdCreate,                 nullptr },
    { "dial",     &SystemEngine::cmdDial,                   nullptr },
    { "dumpp",    &SystemEngine::cmdDump<map::asProgram>,   nullptr },
    { "dumpd",    &SystemEngine::cmdDump<map::asData>,      nullptr },
    { "dumpio",   &SystemEngine::cmdDump<map::asIOPort>,    nullptr },
    { "dumpm",    &SystemEngine::cmdDumpm,                  nullptr },
    { "list",     &SystemEngine::cmdList,                   nullptr },
    { "exit",     &SystemEngine::cmdQuit,                   nullptr },
    { "start",    &SystemEngine::cmdStart,                  nullptr },
    { "quit",     &SystemEngine::cmdQuit,                   nullptr },

    // Terminator
    nullptr
};