// engine.h - system engine package
//
// Date:    May 2, 2023
// Author:  Tim Stark

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

class SystemEngine
{
public:
    enum cmdStatus
    {
        cmdOk = 0,      // Successful
        cmdNotFound,    // Command not found
        cmdShutdown     // Shutdown Requested
    };

    using cmdFunc_t = cmdStatus (SystemEngine::*)(UserConsole *, args_t &);
    using cmdDevice_t = cmdStatus (SystemEngine::*)(UserConsole *, args_t &);

    struct command_t
    {
        cchar_t     *name;
        cmdFunc_t   func;
        command_t   *options;
    };

    // Global initialization routines
    void ginit();
    void gexit();
    
    const SystemDriver *findSystemDriver(cstr_t &name);

    Machine *findSystem(cstr_t &name);
    Device *findDevice(UserConsole *user, cstr_t &name);
    Device *findDevice(cstr_t &name);

    Machine *createMachine(UserConsole *user, cstr_t &devName, cstr_t &sysName);
    void dial(Machine *sysMachine, UserConsole *user);

    int split(const std::string &cmdLine, args_t &args);
    int execute(UserConsole *user, const std::string &cmdLine);

    // Command function calls
    cmdStatus cmdCreate(UserConsole *user, args_t &args);
    cmdStatus cmdDial(UserConsole *user, args_t &args);
    cmdStatus cmdListr(UserConsole *user, args_t &args);
    cmdStatus cmdStart(UserConsole *user, args_t &args);
    cmdStatus cmdQuit(UserConsole *user, args_t &args);

    template <map::AddressType type>
    cmdStatus cmdDumpm(UserConsole *user, args_t &args);

private:
    static command_t mseCommands[];
    
    static const SystemDriver *sysList[];
    static std::vector<Machine *> machines;

    Machine *dialedMachine = nullptr;
    Device *dialedSystem = nullptr;

    // For repeating commands
    cmdFunc_t lastCommand = nullptr;
    Device   *lastDevice = nullptr;
    offs_t    lastAddress = 0;
};