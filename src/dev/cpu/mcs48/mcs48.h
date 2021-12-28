// mcs48.h - MCS 48 processor series package
//
// Author:  Tim Stark
// Date:    12/28/21

#pragma once

#include "emu/devcpu.h"

class mcs48_cpuDevice : public ProcessorDevice
{
public:
    mcs48_cpuDevice(const SystemConfig &config, const DeviceType &type,
        cstag_t &devName, Device *owner, uint64_t clock, int paWidth, int daWidth);
    virtual ~mcs48_cpuDevice() = default;

	// Virtual device function calls
	void devStart() override;

protected:
    map::AddressConfigList getAddressConfigList() const override;

    uint8_t idata[256];

    map::AddressConfig mapProgramConfig;
    map::AddressConfig mapDataConfig;
    map::AddressConfig mapIOPortConfig;

    map::MemoryAccess<12, 0, 0, LittleEndian>::specific mapProgram;
    map::MemoryAccess<8, 0, 0, LittleEndian>::specific mapData;
    map::MemoryAccess<8, 0, 0, LittleEndian>::specific mapIOPort;
};

class i8035_cpuDevice : public mcs48_cpuDevice
{
public:
	i8035_cpuDevice(const SystemConfig &config, cstag_t &tagName, Device *owner, uint64_t clock);
	virtual ~i8035_cpuDevice() = default;

};

class i8039_cpuDevice : public mcs48_cpuDevice
{
public:
	i8039_cpuDevice(const SystemConfig &config, cstag_t &tagName, Device *owner, uint64_t clock);
	virtual ~i8039_cpuDevice() = default;

};

class i8040_cpuDevice : public mcs48_cpuDevice
{
public:
	i8040_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8040_cpuDevice() = default;

};

class i8048_cpuDevice : public mcs48_cpuDevice
{
public:
	i8048_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8048_cpuDevice() = default;

};

class i8049_cpuDevice : public mcs48_cpuDevice
{
public:
	i8049_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8049_cpuDevice() = default;

};

class i8050_cpuDevice : public mcs48_cpuDevice
{
public:
	i8050_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8050_cpuDevice() = default;

};

class i8749_cpuDevice : public mcs48_cpuDevice
{
public:
	i8749_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8749_cpuDevice() = default;

};

class i8750_cpuDevice : public mcs48_cpuDevice
{
public:
	i8750_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8750_cpuDevice() = default;

};

DECLARE_DEVICE_TYPE(i8035, i8035_cpuDevice);
DECLARE_DEVICE_TYPE(i8039, i8039_cpuDevice);
DECLARE_DEVICE_TYPE(i8040, i8040_cpuDevice);
DECLARE_DEVICE_TYPE(i8049, i8049_cpuDevice);
DECLARE_DEVICE_TYPE(i8050, i8050_cpuDevice);
DECLARE_DEVICE_TYPE(i8749, i8749_cpuDevice);
DECLARE_DEVICE_TYPE(i8750, i8750_cpuDevice);
