// main.cpp - main routines
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    Dec 6, 2021

#include "emu/core.h"
#include "main/engine.h"
#include "main/user.h"

int main(int argc, char **argv)
{
    std::cout << "Welcome to Multi-System Emulator System\n" << std::endl;

    SystemEngine engine;
    engine.ginit();

    UserConsole user(engine);
    user.prompt();

    engine.gexit();
    exit(0);
}