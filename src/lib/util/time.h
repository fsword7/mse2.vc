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

    private:
        attoseconds_t time = 0;
    };
}

using attotime_t = emu::time::Attotime;
