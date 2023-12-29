#ifndef NEZUMI_INCLUDE_NEZUMI_BASE_HPP
#define NEZUMI_INCLUDE_NEZUMI_BASE_HPP

#if __cplusplus >= 201402L

#include <concepts>
#include <cstdint>
#include <stdexcept>

namespace nezumi {

constexpr bool cppAtLeast14 = __cplusplus >= 201402L;
constexpr bool cppAtLeast17 = __cplusplus >= 201703L;
constexpr bool cppAtLeast20 = __cplusplus >= 202002L;
constexpr bool cpp14 = cppAtLeast14 && !cppAtLeast17;
constexpr bool cpp17 = cppAtLeast17 && !cppAtLeast20;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef bool b8;
typedef uint32_t b32;

typedef const char *cString;

} // namespace nezumi

#else  // C++11 or older
static_assert(__cplusplus >= 201402L, "Nezumi library requires C++14 or newer");
#endif // C++14 or newer

#endif // NEZUMI_INCLUDE_NEZUMI_BASE_HPP