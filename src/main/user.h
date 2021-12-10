// user.h - user console handler package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

#pragma once

class UserConsole
{
public:
    UserConsole(SystemEngine &engine);
    ~UserConsole();

    void prompt();

private:
    SystemEngine &engine;
};
