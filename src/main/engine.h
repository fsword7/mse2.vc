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

class SystemEngine
{
public:
    using cmdFunc_t = int (SystemEngine::*)(UserConsole *, args_t &);
    using cmdDevice_t = int (SystemEngine::*)(UserConsole *, args_t &);

    struct command_t
    {
        ctag_t *name;
        cmdFunc_t func;
        command_t *options;
    };

    static command_t mseCommands[];

public:
    SystemEngine() = default;
    ~SystemEngine() = default;

    int split(cstag_t &cmdLine, args_t &args);
    void execute(UserConsole *user, std::string cmdLine);

private:
    int cmdCreate(UserConsole *user, args_t &args);


    // command_t mseCommands2[2] =
    // {
    //     { "create", cmdCreate, nullptr },
    //     // Terminator
    //     nullptr
    // };
};