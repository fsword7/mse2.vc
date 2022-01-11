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

Device *SystemEngine::findDevice(cstag_t &name)
{
    if (dialedSystem == nullptr)
        return nullptr;
    
    for (Device &dev : DeviceIterator(*dialedSystem))
        if (dev.getsDeviceName() == name)
            return &dev;

    return nullptr;
}

Device *SystemEngine::findDevice(UserConsole *user, cstag_t &name)
{

    if (dialedSystem == nullptr)
    {
        fmt::printf("Please dial system first\n");
        return nullptr;
    }
    
    for (Device &dev : DeviceIterator(*dialedSystem))
        if (dev.getsDeviceName() == name)
            return &dev;

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
    {
        if (lastCommand != nullptr)
            (this->*lastCommand)(user, args);
        return 0;
    }
    lastCommand = nullptr;
    
    command_t *cmdList = mseCommands;
    cmdStatus status = cmdNotFound; // default resulting status

    for (int idx = 0; cmdList[idx].name; idx++)
    {
        if (cmdList[idx].name == args.getArgument())
        {
            args.getNext();

            if (cmdList[idx].func != nullptr)
                status = (this->*cmdList[idx].func)(user, args);
            if (status == cmdShutdown)
                return 1;
        }
    }

    if (status == cmdNotFound)
        fmt::printf("*** Unknown '%s' command\n", args.getArgument());
    return 0;
}

Machine *SystemEngine::createMachine(UserConsole *user, cstag_t &devName, cstag_t &sysName)
{
    const SystemDriver *driver = findSystemDriver(sysName);
    if (driver == nullptr)
    {
        fmt::printf("%s: system '%s' not recongized.\n", devName, sysName);
        return nullptr;
    }

    Machine *sysMachine = Machine::create(user, driver, devName);

    if (sysMachine != nullptr)
        machines.push_back(sysMachine);
    return sysMachine;
}

void SystemEngine::dial(Machine *sysMachine, UserConsole *user)
{
    assert(user != nullptr);

    // Switch console access for desired system machine.
    if (dialedMachine != nullptr)
        dialedMachine->setConsole(nullptr);
    if (sysMachine != nullptr)
        sysMachine->setConsole(user);

    // Switch system machine and device as current
    dialedMachine = sysMachine;
    dialedSystem = (sysMachine != nullptr)
        ? sysMachine->getSystemDevice() : nullptr;
}