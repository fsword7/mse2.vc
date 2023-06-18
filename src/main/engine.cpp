// engine.cpp - system engine package
//
// Date:    May 2, 2023
// Author:  Tim Stark

#include "emu/core.h"
#include "emu/machine.h"
#include "main/engine.h"

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

const SystemDriver *SystemEngine::findSystemDriver(cstr_t &name)
{
    for (int idx = 0; sysList[idx]; idx++)
    {
        const SystemDriver *driver = sysList[idx];
        if (name == std::string(driver->name))
            return driver;
    }
    return nullptr;
}

Machine *SystemEngine::findSystem(cstr_t &name)
{
    for (auto mach : machines)
        if (name == mach->getDeviceName())
            return mach;
    return nullptr;
}

Device *SystemEngine::findDevice(cstr_t &name)
{
    if (dialedSystem == nullptr)
        return nullptr;
    
    for (Device &dev : DeviceIterator(*dialedSystem))
        if (dev.getsDeviceName() == name)
            return &dev;

    return nullptr;
}

Device *SystemEngine::findDevice(UserConsole *user, cstr_t &name)
{

    if (dialedSystem == nullptr)
    {
        std::cout << fmt::format("Please dial system first\n");
        return nullptr;
    }
    
    for (Device &dev : DeviceIterator(*dialedSystem))
        if (dev.getsDeviceName() == name)
            return &dev;

    return nullptr;
}

int SystemEngine::split(const std::string &cmdLine, args_t &args)
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

int SystemEngine::execute(UserConsole *user, const std::string &cmdLine)
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
        std::cout << fmt::format("*** Unknown '{}' command\n", args.getArgument());
    return 0;
}

Machine *SystemEngine::createMachine(UserConsole *user, cstr_t &devName, cstr_t &sysName)
{
    cSystemDriver *driver = findSystemDriver(sysName);
    if (driver == nullptr)
    {
        std::cout << fmt::format("{}: system '{}' not recongized.\n", devName, sysName);
        return nullptr;
    }

    Machine *sysMachine = new Machine(user, *driver, devName);

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

    // // Switch system machine and device as current
    dialedMachine = sysMachine;
    dialedSystem = (sysMachine != nullptr)
        ? sysMachine->getSystemDevice() : nullptr;
}