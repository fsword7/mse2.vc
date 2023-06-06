// templates.h
//
// Author:  Tim Stark
// Date:    Jun 6, 2023

#pragma once

template <class DeviceClass> template <typename... Args>
inline DeviceClass *DeviceCreator<DeviceClass>::operator ()(SystemConfig &config, cstr_t &devName, Args &&... args) const
{
    return dynamic_cast<DeviceClass *>(config.addDeviceType(devName, *this, std::forward<Args>(args)...));
}

template <class DeviceClass> template <typename Exposed, bool Required, typename... Args>
DeviceClass &DeviceCreator<DeviceClass>::operator ()(SystemConfig &config,
    DeviceFinder<Exposed, Required> &finder, cstr_t &devName, Args &&... args) const
{
    DeviceClass &device(dynamic_cast<DeviceClass &>(*config.addDeviceType(devName, *this, std::forward<Args>(args)...)));

    finder = device;
    return device;
}