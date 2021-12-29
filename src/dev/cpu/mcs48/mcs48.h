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
        cstag_t &devName, Device *owner, uint64_t clock, int paWidth, int daWidth, int romSize);
    virtual ~mcs48_cpuDevice() = default;

	// Virtual device function calls
	void devStart() override;

protected:
    map::AddressConfigList getAddressConfigList() const override;
	
	void setProgram1024(map::AddressList &map);
	void setProgram2048(map::AddressList &map);
	void setProgram4096(map::AddressList &map);

	void setData64(map::AddressList &map);
	void setData128(map::AddressList &map);
	void setData256(map::AddressList &map);

	// RequiredSharedPointer<uint8_t> idata;

	uint16_t iromSize; // Internal ROM size (1024, 2048 or 4096 bytes)
	uint16_t iramSize; // Internal RAM size (64, 128 or 256 bytes)
	
    map::AddressConfig mapProgramConfig;
    map::AddressConfig mapDataConfig;
    map::AddressConfig mapIOPortConfig;

    map::MemoryAccess<12, 0, 0, LittleEndian>::specific mapProgram;
    map::MemoryAccess<8, 0, 0, LittleEndian>::specific mapData;
    map::MemoryAccess<8, 0, 0, LittleEndian>::specific mapIOPort;
};

class i8021_cpuDevice : public mcs48_cpuDevice
{
public:
	i8021_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8021_cpuDevice() = default;
};

class i8022_cpuDevice : public mcs48_cpuDevice
{
public:
	i8022_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8022_cpuDevice() = default;
};

class i8035_cpuDevice : public mcs48_cpuDevice
{
public:
	i8035_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8035_cpuDevice() = default;
};

class i8039_cpuDevice : public mcs48_cpuDevice
{
public:
	i8039_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
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

class i8648_cpuDevice : public mcs48_cpuDevice
{
public:
	i8648_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8648_cpuDevice() = default;
};

class i8748_cpuDevice : public mcs48_cpuDevice
{
public:
	i8748_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8748_cpuDevice() = default;
};

class i8049_cpuDevice : public mcs48_cpuDevice
{
public:
	i8049_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8049_cpuDevice() = default;
};

class i8749_cpuDevice : public mcs48_cpuDevice
{
public:
	i8749_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8749_cpuDevice() = default;
};

class i8050_cpuDevice : public mcs48_cpuDevice
{
public:
	i8050_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8050_cpuDevice() = default;
};

class i8750_cpuDevice : public mcs48_cpuDevice
{
public:
	i8750_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
	virtual ~i8750_cpuDevice() = default;
};


// Intel UPI-41 processor series

class upi41_cpuDevice : public mcs48_cpuDevice
{
protected:
	upi41_cpuDevice(const SystemConfig &config, const DeviceType &type, cstag_t &devName,
		Device *owner, uint64_t clock, int paWidth, int daWidth, int romSize)
	: mcs48_cpuDevice(config, type, devName, owner, clock, paWidth, daWidth, romSize)
	{ }
};

class i8041a_cpuDevice : public upi41_cpuDevice
{
public:
	i8041a_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
};

class i8741a_cpuDevice : public upi41_cpuDevice
{
public:
	i8741a_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
};

class i8041ah_cpuDevice : public upi41_cpuDevice
{
public:
	i8041ah_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
};

class i8741ah_cpuDevice : public upi41_cpuDevice
{
public:
	i8741ah_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
};

class i8042_cpuDevice : public upi41_cpuDevice
{
public:
	i8042_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
};

class i8742_cpuDevice : public upi41_cpuDevice
{
public:
	i8742_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
};

class i8042ah_cpuDevice : public upi41_cpuDevice
{
public:
	i8042ah_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
};

class i8742ah_cpuDevice : public upi41_cpuDevice
{
public:
	i8742ah_cpuDevice(const SystemConfig &config, cstag_t &devName, Device *owner, uint64_t clock);
};

// Intel MCS-48 processors
DECLARE_DEVICE_TYPE(I8021,   i8021_cpuDevice);
DECLARE_DEVICE_TYPE(I8022,   i8022_cpuDevice);
DECLARE_DEVICE_TYPE(I8035,   i8035_cpuDevice);
DECLARE_DEVICE_TYPE(I8039,   i8039_cpuDevice);
DECLARE_DEVICE_TYPE(I8040,   i8040_cpuDevice);
DECLARE_DEVICE_TYPE(I8048,   i8048_cpuDevice);
DECLARE_DEVICE_TYPE(I8648,   i8648_cpuDevice);
DECLARE_DEVICE_TYPE(I8748,   i8748_cpuDevice);
DECLARE_DEVICE_TYPE(I8049,   i8049_cpuDevice);
DECLARE_DEVICE_TYPE(I8749,   i8749_cpuDevice);
DECLARE_DEVICE_TYPE(I8050,   i8050_cpuDevice);
DECLARE_DEVICE_TYPE(I8750,   i8750_cpuDevice);

// Intel UPI-41 processors
DECLARE_DEVICE_TYPE(I8041A,  i8041a_cpuDevice);
DECLARE_DEVICE_TYPE(I8741A,  i8741a_cpuDevice);
DECLARE_DEVICE_TYPE(I8041AH, i8041ah_cpuDevice);
DECLARE_DEVICE_TYPE(I8741AH, i8741ah_cpuDevice);
DECLARE_DEVICE_TYPE(I8042,   i8042_cpuDevice);
DECLARE_DEVICE_TYPE(I8742,   i8742_cpuDevice);
DECLARE_DEVICE_TYPE(I8042AH, i8042ah_cpuDevice);
DECLARE_DEVICE_TYPE(I8742AH, i8742ah_cpuDevice);