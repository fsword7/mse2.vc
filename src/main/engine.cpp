// engine.h - User engine package
//
// Author:  Tim Stark
// Date:    12/7/2021

#include "emu/core.h"
#include "emu/machine.h"
#include "main/engine.h"
#include "main/user.h"

std::vector<Machine *> SystemEngine::machines;

void SystemEngine::ginit()
{
    machines.clear();
}

void SystemEngine::gexit()
{
    for (auto mach : machines)
        delete mach;
    machines.clear();
}

const SystemDriver *SystemEngine::findSystemDriver(cstag_t &name)
{
    for (int idx = 0; sysList[idx]; idx++)
    {
        const SystemDriver *driver = sysList[idx];
        if (name == std::string(driver->name))
            return driver;
    }
    return nullptr;
}

Machine *SystemEngine::findSystem(cstag_t &name)
{
    for (auto mach : machines)
        if (name == mach->getDeviceName())
            return mach;
    return nullptr;
}

int SystemEngine::split(cstag_t &cmdLine, args_t &args)
{
    std::istringstream line(cmdLine);

    while (line)
    {
        std::string word;
        line >> word;
        if (!word.empty())
            args.add(word);
    }

    return args.getSize();
}

int SystemEngine::execute(UserConsole *user, std::string cmdLine)
{
    args_t args;

    args.clear();
    split(cmdLine, args);

    if (args.isEmpty())
        return 0;

    command_t *cmdList = mseCommands;
    for (int idx = 0; cmdList[idx].name; idx++)
    {
        if (cmdList[idx].name == args.getArgument())
        {
            args.getNext();

            if (cmdList[idx].func != nullptr)
                return cmdList[idx].func(user, this, args);
        }
    }

    fmt::printf("*** Unknown '%s' command\n", args.getArgument());
    return 0;
}

int SystemEngine::createMachine(UserConsole *user, cstag_t &devName, cstag_t &sysName)
{
    const SystemDriver *driver = findSystemDriver(sysName);
    if (driver == nullptr)
    {
        fmt::printf("%s: system '%s' not recongized.\n", devName, sysName);
        return 0;
    }

    Machine *sysMachine = Machine::create(user, driver, devName);

    machines.push_back(sysMachine);
    return 0;
}

// int SystemEngine::cmdCreate(UserConsole *user, args_t &args)
// {
//     fmt::printf("Here is create command\n");
//     return 0;
// }

// SystemEngine::command_t SystemEngine::mseCommands[] =
// {
//     { "create",     SystemEngine::cmdCreate,    nullptr },
//     // Terminator
//     nullptr
// };
