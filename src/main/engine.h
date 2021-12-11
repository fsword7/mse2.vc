// engine.h - User engine package
//
// Author:  Tim Stark
// Date:    12/7/2021

#pragma once

class args_t
{
public:
    args_t() = default;
    ~args_t() = default;

    using args = std::vector<std::string>;

    inline void add(std::string arg)    { params.push_back(arg); count++; }
    inline void clear()                 { params.clear(); }
    inline bool isEmpty()               { return params.empty(); }
    
    inline int getSize() const { return params.size(); }

    inline std::string operator [](int idx) { return (idx < count) ? params[idx] : ""; }

    inline std::string getArgument()    { return (index < count) ? params[index] : ""; }
    inline std::string getNext()        { return (index < count) ? params[index++] : ""; }

private:
    int  count = 0;
    int  index = 0;
    args params;
};

class UserConsole;
class SystemEngine;
class Machine;

class SystemEngine
{
public:
    enum cmdStatus
    {
        cmdOk = 0,   // Successful
        cmdNotFound, // Command not found (unknown)
        cmdShutdown, // Multi-System Emulator Shutdown
    };

    using cmdFunc_t = cmdStatus (SystemEngine::*)(UserConsole *, args_t &);
    using cmdDevice_t = cmdStatus (SystemEngine::*)(UserConsole *, args_t &);

    struct command_t
    {
        ctag_t *name;
        cmdFunc_t func;
        command_t *options;
    };

public:
    SystemEngine() = default;
    ~SystemEngine() = default;

    // Global initialization routines
    void ginit();
    void gexit();

    Machine *findSystem(cstag_t &name);
    const SystemDriver *findSystemDriver(cstag_t &name);

    bool createMachine(UserConsole *user, cstag_t &devName, cstag_t &sysName);

    int split(cstag_t &cmdLine, args_t &args);
    int execute(UserConsole *user, std::string cmdLine);

private:
    // Command function calls
    cmdStatus cmdCreate(UserConsole *user, args_t &args);
    cmdStatus cmdQuit(UserConsole *user, args_t &args);

    static command_t mseCommands[];

    static const SystemDriver *sysList[];
    static std::vector<Machine *> machines;
};
