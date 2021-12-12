// i8080.h - Intel 8080/8085 Processor package
//
// Author:  Tim Stark
// Date:    12/10/2021

#pragma once

class i8080_cpuDevice : public ProcessorDevice
{
public:
    i8080_cpuDevice(const SystemConfig &config, const DeviceType &type,
        cstag_t &devName, Device *owner, uint64_t clock);
    i8080_cpuDevice(const SystemConfig &config, cstag_t &devName,
        Device *owner, uint64_t clock);
    virtual ~i8080_cpuDevice() = default;

protected:
    map::AddressConfigList getAddressConfigList() const override;

    map::AddressConfig mapProgramConfig;
    map::AddressConfig mapIOPortConfig;
};

class i8080a_cpuDevice : public i8080_cpuDevice
{
public:
	i8080a_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8080a_cpuDevice() = default;

};

class i8085_cpuDevice : public i8080_cpuDevice
{
public:
	i8085_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8085_cpuDevice() = default;

};

DECLARE_DEVICE_TYPE(i8080, i8080_cpuDevice);
DECLARE_DEVICE_TYPE(i8080a, i8080a_cpuDevice);
DECLARE_DEVICE_TYPE(i8085, i8085_cpuDevice);