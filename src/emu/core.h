// core.h -  MSE core defintions
//
// Date:    Apr 30, 2023
// Author:  Tim Stark

#include <iostream>
#include <sstream>

#include <string>
#include <vector>
#include <stack>

#include <type_traits>
#include <functional>

#include <cassert>

#include <fmt/core.h>


#define LSB_FIRST 1

enum endian_t { LittleEndian, BigEndian };

#if LSB_FIRST
constexpr const endian_t endianNative = LittleEndian;
#else
constexpr const endian_t endianNative = BigEndian;
#endif

// Determine endian-based values
#define ENDIAN_VALUE(endian, leValue, beValue)  ((endian == LittleEndian) ? (leValue) : (beValue))
#define NATIVE_ENDIAN(leValue, beValue)         ENDIAN_VALUE(endianNative, leValue, beValue)


using cchar_t = const char;
using str_t = std::string;
using cstr_t = const std::string;

using offs_t = uint64_t;

#include "emu/forward.h"
#include "lib/util/list.h"
#include "emu/delegate.h"
#include "emu/driver.h"
#include "emu/sysconfig.h"
#include "emu/device.h"
