// user.h - user console handler package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

#include "emu/core.h"
#include "main/engine.h"
#include "main/user.h"

UserConsole::UserConsole()
{
}

UserConsole::~UserConsole()
{
}

void UserConsole::prompt()
{
    std::string cmdLine;

    while (1)
    {
        std::cout << "MSE> " /* fmt::sprintf("MSE> ") */ << std::flush; 
        getline(std::cin, cmdLine);

        engine.execute(this, cmdLine);
    } 
};