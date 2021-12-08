// engine.h - User engine package
//
// Author:  Tim Stark
// Date:    12/7/2021

#include "emu/core.h"
#include "main/engine.h"
#include "main/user.h"

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

void SystemEngine::execute(UserConsole *user, std::string cmdLine)
{
    args_t args;

    args.clear();
    split(cmdLine, args);

    if (args.isEmpty())
    ;

    command_t *cmd = &mseCommands[0];
    cmd->func(user, this, args);
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
