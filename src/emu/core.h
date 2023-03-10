// core.h - main core definitions package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    Dec 6, 2021

#include <iostream>
#include <fstream>

#include <vector>
#include <stack>
#include <map>
#include <functional>

#include <filesystem>
#include <type_traits>

#include <cstdio>
#include <cassert>

#include <fmt/printf.h>

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

namespace fs = std::filesystem;

// Using 64-bit addressing offset
using offs_t = uint64_t;

template <typename T, typename U> constexpr T makeBitmask(U n)
{
    return T((n < (8 * sizeof(T)) ? (std::make_unsigned_t<T>(1) << n) : std::make_unsigned_t<T>(0)) - 1);
}

typedef char                tag_t;
typedef const char          cchar_t;
typedef const char          ctag_t;

typedef std::string         stag_t;
typedef const std::string   cstag_t;

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

// OSD core package
#include "lib/util/osdcore.h"
#include "lib/util/pair.h"
#include "lib/util/color.h"
#include "lib/util/time.h"

// Common packages
#include "emu/delegate.h"
#include "emu/map/fw.h"
#include "emu/driver.h"
#include "emu/sysconfig.h"
#include "emu/device.h"
#include "emu/devfind.h"
#include "emu/templates.h"
