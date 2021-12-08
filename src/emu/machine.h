// machine.h - machine engine package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

class Machine
{
public:
    Machine(const SystemConfig &config, cstag_t &sysName);
    ~Machine();

private:
    const SystemConfig &config;
    cstag_t sysName;
    Device *sysDevice = nullptr;

};