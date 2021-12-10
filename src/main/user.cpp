// user.h - user console handler package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

#include "emu/core.h"
#include "main/engine.h"
#include "main/user.h"

UserConsole::UserConsole(SystemEngine &engine)
: engine(engine)
{
}

UserConsole::~UserConsole()
{
}

void UserConsole::prompt()
{
    std::string cmdLine;
    bool running = true;

    while (running)
    {
        std::cout << "MSE> " /* fmt::sprintf("MSE> ") */ << std::flush; 
        getline(std::cin, cmdLine);

        if (engine.execute(this, cmdLine))
            running = false;
    } 
};