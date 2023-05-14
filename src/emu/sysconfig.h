// sysconfig.h - system configuration package
//
// Date:    May 2, 2023
// Author:  Tim Stark

#pragma once
class SystemConfig
{
public:
    SystemConfig(cSystemDriver &driver, cstr_t &name);

    Device *createSystemDevice(const DeviceType &type, cstr_t &name, uint64_t clock = 0);
    Device *addDevice(Device *dev, Device *owner = nullptr);

    template <typename Creator, typename... Args>
    auto *addDeviceType(cstr_t &devName, Creator &&type, Args &&... args)
    {
        Device *owner = sysDevice;

        std::cout << fmt::format("{}: Creating {} device...\n", devName, type.getShortName());
        auto dev = type.create(*this, devName, owner, std::forward<Args>(args)...);
        return addDevice(dev, owner);
    }

    // Getter function calls
    inline Device *getSystemDevice() const  { return sysDevice; }
    inline Device *getCurrentDevice() const { return curDevice; }
    inline Device *getConfigDevice() const  { return !cfgDevice.empty() ? cfgDevice.top() : nullptr; }
    
    inline const SystemDriver &getSystemDriver() const { return driver; }

    void setPerfectQuantum(Device &device, cstr_t &devName);

private:
    const SystemDriver &driver;

    Device *sysDevice = nullptr;
    Device *curDevice = nullptr;
    std::stack<Device *> cfgDevice;

    std::pair<Device *, std::string> perfectQuantumDevice;
};

using cSystemConfig = const SystemConfig;