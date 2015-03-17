#pragma once

#include <cstdint>
#include <cstdlib>
#include <functional>
#include <vector>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float f32;
typedef double f64;

#if defined(__GNUC__) || defined(__GNUG__)
#define bswap32 __builtin_bswap32
#define bswap64 __builtin_bswap64
#elif defined(_MSC_VER)
#include <intrin.h>
#define bswap32 _byteswap_ulong
#define bswap64 _byteswap_uint64
#endif
