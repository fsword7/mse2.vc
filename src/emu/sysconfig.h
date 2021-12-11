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

    Device *createSystemDevice(const DeviceType &type, cstag_t &name, uint64_t clock = 0);
    Device *addDevice(Device *dev, Device *owner = nullptr);

    template <typename Creator, typename... Args>
    auto *addDeviceType(cstag_t &devName, Creator &&type, Args &&... args)
    {
        Device *owner = sysDevice;

        fmt::printf("%s: Creating %s device...\n", devName, type.getShortName());
        auto dev = type.create(*this, devName, owner, std::forward<Args>(args)...);
        return addDevice(dev, owner);
    }

    // Getter function calls
    inline Device *getSystemDevice() const { return sysDevice; }
    inline Device *getConfigDevice() const { return !cfgDevice.empty() ? cfgDevice.top() : nullptr; }
    
    inline const SystemDriver &getSystemDriver() const { return driver; }

private:
    const SystemDriver &driver;

    Device *sysDevice = nullptr;
    std::stack<Device *> cfgDevice;
};