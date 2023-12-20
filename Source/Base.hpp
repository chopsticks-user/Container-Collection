#ifndef CONTAINER_SOURCE_BASE_HPP
#define CONTAINER_SOURCE_BASE_HPP

#include <cstdint>
#include <stdexcept>

namespace container {

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

namespace detail {

#ifndef CONTAINER_ALLOW_EXCEPTIONS
constexpr bool allowExceptions = true;
#else
constexpr bool allowExceptions = false;
#endif // CONTAINER_ALLOW_EXCEPTIONS

#define CONTAINER_NOEXCEPT noexcept(!detail::allowExceptions)

template <typename ExceptionType = std::runtime_error>
void expect(bool condition, cString exceptionMessage) {
  if (!condition) {
    throw ExceptionType(exceptionMessage);
  }
}

} // namespace detail

} // namespace container

#endif // CONTAINER_SOURCE_BASE_HPP