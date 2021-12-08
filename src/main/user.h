// user.h - user console handler package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

#pragma once

class SystemEngine;

class UserConsole
{
public:
    UserConsole() = default;
    ~UserConsole() = default;

    void prompt();

private:
    SystemEngine *engine = nullptr;
};