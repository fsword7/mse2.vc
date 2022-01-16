// time.h - Attotime package 
//
// Author:  Tim Stark
// Date:    12/23/21

#pragma once

typedef __int128_t attoseconds_t;
typedef int64_t seconds_t;

constexpr const attoseconds_t ATTOSECOND_PER_SECOND = 1'000'000'000'000'000'000;
constexpr const attoseconds_t ATTOSECOND_PER_MILLISECOND = ATTOSECOND_PER_SECOND / 1'000;
constexpr const attoseconds_t ATTOSECOND_PER_MICROSECOND = ATTOSECOND_PER_SECOND / 1'000'000;
constexpr const attoseconds_t ATTOSECOND_PER_NANOSECOND = ATTOSECOND_PER_SECOND / 1'000'000'000;

constexpr const attoseconds_t ATTOTIME_MAX_SECONDS = ATTOSECOND_PER_SECOND * 1'000'000'000;
constexpr const attoseconds_t ATTOTIME_ZERO = 0;
constexpr const attoseconds_t ATTOTIME_NEVER = ATTOTIME_MAX_SECONDS;

template <typename T>
inline constexpr attoseconds_t HZ_TO_ATTOSECONDS(T &&val) { return ATTOSECOND_PER_SECOND / (val); }

template <typename T>
inline constexpr double ATTOSECONDS_TO_HZ(T &&val) { return double(ATTOSECOND_PER_SECOND) / double(val); }

namespace emu::time
{
    class Attotime
    {
    public:
        Attotime() = default;
        Attotime(attoseconds_t newTime) : time(newTime) { }
        
        static const Attotime zero;
        static const Attotime never;

        inline bool isZero()  { return time == ATTOTIME_ZERO; }
        inline bool isNever() { return time == ATTOTIME_NEVER; }

        attoseconds_t time = 0;
    };
}

using attotime_t = emu::time::Attotime;

inline attotime_t operator + (const attotime_t &left, const attotime_t &right)
{
    attotime_t result;

    // Return as never result
    if (left.time >= ATTOTIME_MAX_SECONDS || right.time >= ATTOTIME_MAX_SECONDS)
        return attotime_t::never;
    result.time = left.time + right.time;
    if (result.time >= ATTOTIME_NEVER)
        return attotime_t::never;
    return result;
}

inline attotime_t operator - (const attotime_t &left, const attotime_t &right)
{
    attotime_t result;

    // Return as never result
    if (left.time >= ATTOTIME_MAX_SECONDS)
        return attotime_t::never;
    result.time = left.time - right.time;
    return result;
}

inline attotime_t operator * (uint64_t factor, const attotime_t &right)
{
    attotime_t result;

    // Return as never result
    if (right.time >= ATTOTIME_MAX_SECONDS)
        return attotime_t::never;
    result.time = right.time * factor;
    return result;
}

inline attotime_t operator * (const attotime_t &left, uint64_t factor)
{
    attotime_t result;

    // Return as never result
    if (left.time >= ATTOTIME_MAX_SECONDS)
        return attotime_t::never;
    result.time = left.time * factor;
    return result;
}

inline attotime_t operator / (const attotime_t &left, uint64_t factor)
{
    attotime_t result;

    // Return as never result
    if (left.time >= ATTOTIME_MAX_SECONDS)
        return attotime_t::never;
    if (factor == 0)
        return left;
    result.time = left.time / factor;
    return result;
}
