// devsys.h - system device package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    12/7/2021

class SystemDevice : public Device
{
public:
    SystemDevice(const SystemConfig &config, const DeviceType &type, cstag_t &devName, uint64_t clock);

    
};