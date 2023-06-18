// core.h -  MSE core defintions
//
// Date:    Apr 30, 2023
// Author:  Tim Stark

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>

#include <string>
#include <vector>
#include <stack>
#include <map>

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

namespace fs = std::filesystem;

template <typename T, typename U> constexpr T makeBitmask(U n)
{
    return T((n < (8 * sizeof(T)) ? (std::make_unsigned_t<T>(1) << n) : std::make_unsigned_t<T>(0)) - 1);
}

namespace map
{
    enum AddressType
    {
        asProgram = 0,  // Program address space
        asData,         // Data address space
        asIOPort        // I/O port address space
    };
}

template <typename Dest, typename Source>
inline Dest mse_static_cast(Source *src)
{
#ifdef ENABLE_DEBUG
#endif /* ENABLE_DEBUG */
	return static_cast<Dest>(src);
}

template <class Dest, class Source>
inline Dest mse_static_cast(Source &src)
{
#ifdef ENABLE_DEBUG
#endif /* ENABLE_DEBUG */
	return static_cast<Dest>(src);
}

#include "emu/forward.h"
#include "lib/util/list.h"
#include "emu/delegate.h"
#include "emu/map/fw.h"
#include "emu/driver.h"
#include "emu/sysconfig.h"
#include "emu/device.h"
#include "emu/objfinder.h"

#include "emu/templates.h"

