// user.h - user console package
//
// Date:    May 2, 2023
// Author:  Tim Stark

#pragma once
class UserConsole
{
public:
    UserConsole(SystemEngine &);
    ~UserConsole() = default;

    void prompt();

private:
    SystemEngine &engine;
};