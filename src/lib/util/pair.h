// pair.h - pair/scale variable package
//
// Author:  Tim Stark
// Date:    12/23/21

// sign extension macro definition
#define SXTB(val)   int8_t(val)
#define SXTW(val)   int16_t(val)
#define SXTL(val)   int32_t(val)
#define SXTQ(val)   int64_t(val)

// zero extension macro definiton
#define ZXTB(val)   uint8_t(val)
#define ZXTW(val)   uint16_t(val)
#define ZXTL(val)   uint32_t(val)
#define ZXTQ(val)   uint64_t(val)

// varying pair/scale 16-bit integer
union pair16_t
{
#if LSB_FIRST
    struct { int8_t l, h; } sb;
    struct { uint8_t l, h; } ub;
#else
    struct { int8_t h, l; } sb;
    struct { uint8_t h, l; } ub;
#endif /* LSB_FIRST */
    int16_t sw;
    uint16_t uw;
};

// varying pair/scale 32-bit integer
union pair32_t
{
#if LSB_FIRST
    struct { int8_t l, h, h2, h3; } sb;
    struct { uint8_t l, h, h2, h3; } ub;
    struct { int16_t l, h; } sw;
    struct { uint16_t l, h; } uw;
#else
    struct { int8_t h3, h2, h, l; } sb;
    struct { uint8_t h3, h2, h, l; } ub;
    struct { int16_t h, l; } sw;
    struct { uint16_t h, l; } uw;
#endif /* LSB_FIRST */
    int32_t sd;
    uint32_t ud;
};

// varying pair/scale 64-bit integer
union pair64_t
{
#if LSB_FIRST
    struct { int8_t l, h, h2, h3, h4, h5, h6, h7; } sb;
    struct { uint8_t l, h, h2, h3, h4, h5, h6, h7; } ub;
    struct { int16_t l, h, h2, h3; } sw;
    struct { uint16_t l, h, h2, h3; } uw;
    struct { int32_t l, h; } sd;
    struct { uint32_t l, h; } ud;
#else
    struct { int8_t h7, h6, h5, h4, h3, h2, h, l; } sb;
    struct { uint8_t h7, h6, h5, h4, h3, h2, h, l; } ub;
    struct { int16_t h3, h2, h, l; } sw;
    struct { uint16_t h3, h2, h, l; } uw;
    struct { int32_t h, l; } sd;
    struct { uint32_t h, l; } ud;
#endif /* LSB_FIRST */
    int64_t sq;
    uint64_t uq;
};

// varying scale unsigned 16-bit integer
struct scale16_t
{
    union {
        uint16_t w;
        uint8_t  b;
    };
};

// varying scale unsigned 32-bit integer
struct scale32_t
{
    union {
        uint32_t d;
        uint16_t w;
        uint8_t  b;
    };
};

// varying scale unsigned 64-bit integer
struct scale64_t
{
    union {
        uint64_t q;
        uint32_t d;
        uint16_t w;
        uint8_t  b;
    };
};