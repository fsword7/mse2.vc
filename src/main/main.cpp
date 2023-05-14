// main.cpp - main routine package
//
// Date:    Apr 30, 2023
// Author:  Tim Stark

#include "emu/core.h"
#include "main/engine.h"
#include "main/user.h"

int main(int argc, char **argv)
{
    std::cout << fmt::format("Welcome to Multi-system Emulator System\n\n");

    SystemEngine engine;
    engine.ginit();

    UserConsole user(engine);
    user.prompt();

    engine.gexit();
    exit(0);

    return 0;
}