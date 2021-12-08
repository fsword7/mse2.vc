// sysconfig.h - system configuration package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    Dec 6, 2021

class Device;
class DeviceType;

class SystemConfig
{
public:
    SystemConfig(const SystemDriver &driver, cstag_t &name);

    Device *addDeviceType(const DeviceType &type, cstag_t &name, uint64_t clock = 0);
    Device *addDevice(Device *dev, Device *owner = nullptr);

    // Getter function calls
    inline Device *getSystemDevice() const { return sysDevice; }
    inline Device *getCOnfigDevice() const { return cfgDevice; }
    
private:
    Device *sysDevice = nullptr;
    Device *cfgDevice = nullptr;
};