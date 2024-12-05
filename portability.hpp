#ifndef JSONIFIER_PORTABILITY_H
#define JSONIFIER_PORTABILITY_H

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cfloat>
#include <cassert>
#ifndef _WIN32
// strcasecmp, strncasecmp
#include <strings.h>
#endif

// We are using size_t without namespace std:: throughout the project
using std::size_t;

#ifdef _MSC_VER
#define JSONIFIER_VISUAL_STUDIO 1
/**
 * We want to differentiate carefully between
 * clang under visual studio and regular visual
 * studio.
 *
 * Under clang for Windows, we enable:
 *  * target pragmas so that part and only part of the
 *     code gets compiled for advanced instructions.
 *
 */
#ifdef __clang__
 // clang under visual studio
#define JSONIFIER_CLANG_VISUAL_STUDIO 1
#else
 // just regular visual studio (best guess)
#define JSONIFIER_REGULAR_VISUAL_STUDIO 1
#endif // __clang__
#endif // _MSC_VER

#if (defined(__x86_64__) || defined(_M_AMD64)) && !defined(_M_ARM64EC)
#define JSONIFIER_IS_X86_64 1
#elif defined(__aarch64__) || defined(_M_ARM64) || defined(_M_ARM64EC)
#define JSONIFIER_IS_ARM64 1
#elif defined(__riscv) && __riscv_xlen == 64
#define JSONIFIER_IS_RISCV64 1
#elif defined(__loongarch_lp64)
#define JSONIFIER_IS_LOONGARCH64 1
#elif defined(__PPC64__) || defined(_M_PPC64)
#if defined(__ALTIVEC__)
#define JSONIFIER_IS_PPC64_VMX 1
#endif // defined(__ALTIVEC__)
#else
#define JSONIFIER_IS_32BITS 1

#if defined(_M_IX86) || defined(__i386__)
#define JSONIFIER_IS_X86_32BITS 1
#elif defined(__arm__) || defined(_M_ARM)
#define JSONIFIER_IS_ARM_32BITS 1
#elif defined(__PPC__) || defined(_M_PPC)
#define JSONIFIER_IS_PPC_32BITS 1
#endif

#endif // defined(__x86_64__) || defined(_M_AMD64)

// Our fast kernels require 64-bit systems.
//
// On 32-bit x86, we lack 64-bit popcnt, lzcnt, blsr instructions.
// Furthermore, the number of SIMD registers is reduced.
//
// On 32-bit ARM, we would have smaller registers.
//
// The simdjson users should still have the fallback kernel. It is
// slower, but it should run everywhere.

// workaround for large stack sizes under -O0.
// https://github.com/simdjson/simdjson/issues/691
#ifdef __APPLE__
#ifndef __OPTIMIZE__
#undef JSONIFIER_THREADS_ENABLED
#endif
#endif

#if defined(NDEBUG) || defined(__OPTIMIZE__) || (defined(_MSC_VER) && !defined(_DEBUG))
// If NDEBUG is set, or __OPTIMIZE__ is set, or we are under MSVC in release mode,
// then do away with asserts and use __assume.
#if JSONIFIER_VISUAL_STUDIO
#define JSONIFIER_UNREACHABLE() __assume(0)
#define JSONIFIER_ASSUME(COND) __assume(COND)
#else
#define JSONIFIER_UNREACHABLE() __builtin_unreachable();
#define JSONIFIER_ASSUME(COND) do { if (!(COND)) __builtin_unreachable(); } while (0)
#endif

#else // defined(NDEBUG) || defined(__OPTIMIZE__) || (defined(_MSC_VER) && !defined(_DEBUG))
// This should only ever be enabled in debug mode.
#define JSONIFIER_UNREACHABLE() assert(0);
#define JSONIFIER_ASSUME(COND) assert(COND)

#endif

#endif // JSONIFIER_PORTABILITY_H