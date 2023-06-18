// forward.h - Class forwarding package
//
// Date:    May 2, 2023
// Author:  Tim Stark

#pragma once

class SystemEngine;
class SystemDriver;
class UserConsole;
class SystemConfig;
class ObjectFinder;
class Device;
class DeviceInterface;
class DeviceType;
class Machine;
class CPUDevice;

class diMemory;

template <typename Exposed, bool Required>
class DeviceFinder;

namespace map
{
    class AddressEntry;
    class AddressList;

    class AddressConfig;
    class AddressSpace;

    template <int Level,  int dWidth, int aShift, endian_t eType>
    class MemoryAccessSpecific;

    class MemoryManager;
    class MemoryBlock;
    class MemoryRegion;
    class MemoryShare;
    class MemoryBank;

}