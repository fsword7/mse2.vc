// user.cpp - user console package
//
// Date:    May 2, 2023
// Author:  Tim Stark

#include "emu/core.h"
#include "main/engine.h"
#include "main/user.h"

UserConsole::UserConsole(SystemEngine &engine)
: engine(engine)
{
}

void UserConsole::prompt()
{
    std::string cmdLine;
    bool running = true;

    while (running)
    {
        std::cout << fmt::format("MSE> ") << std::flush;
        getline(std::cin, cmdLine);

        if (engine.execute(this, cmdLine))
            running = false;
    } 
};