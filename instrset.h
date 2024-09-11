/****************************  instrset.h   **********************************
* Author:        Agner Fog
* Date created:  2012-05-30
* Last modified: 2023-12-02
* Version:       2.02.02
* Project:       vector class library
* Description:
* Header file for various compiler-specific tasks as well as common
* macros and templates. This file contains:
*
* > Selection of the supported instruction set
* > Defines compiler version macros
* > Undefines certain macros that prevent function overloading
* > Helper functions that depend on instruction set, compiler, or platform
* > Common templates for permute, blend, etc.
*
* For instructions, see vcl_manual.pdf
*
* (c) Copyright 2012-2023 Agner Fog.
* Apache License version 2.0 or later.
******************************************************************************/

#ifndef INSTRSET_H
#define INSTRSET_H 20200

// check if compiled for C++17
#if defined(_MSVC_LANG)  // MS compiler has its own version of __cplusplus with different value
#if _MSVC_LANG < 201703
#error Please compile for C++17 or higher
#endif
#else  // all other compilers
#if __cplusplus < 201703
#error Please compile for C++17 or higher
#endif
#endif

// Allow the use of floating point permute instructions on integer vectors.
// Some CPU's have an extra latency of 1 or 2 clock cycles for this, but
// it may still be faster than alternative implementations:
#define ALLOW_FP_PERMUTE  true


// Macro to indicate 64 bit mode
#if (defined(_M_AMD64) || defined(_M_X64) || defined(__amd64) ) && ! defined(__x86_64__)
#define __x86_64__ 1  // There are many different macros for this, decide on only one
#endif

// The following values of INSTRSET are currently defined:
// 2:  SSE2
// 3:  SSE3
// 4:  SSSE3
// 5:  SSE4.1
// 6:  SSE4.2
// 7:  AVX
// 8:  AVX2
// 9:  AVX512F
// 10: AVX512BW/DQ/VL
// In the future, INSTRSET = 11 may include AVX512VBMI and AVX512VBMI2, but this
// decision cannot be made before the market situation for CPUs with these
// instruction sets is better known

// Find instruction set from compiler macros if INSTRSET is not defined.
// Note: Some of these macros are not defined in Microsoft compilers
#ifndef INSTRSET
#if defined ( __AVX512VL__ ) && defined ( __AVX512BW__ ) && defined ( __AVX512DQ__ )
#define INSTRSET 10
#elif defined ( __AVX512F__ ) || defined ( __AVX512__ )
#define INSTRSET 9
#elif defined ( __AVX2__ )
#define INSTRSET 8
#elif defined ( __AVX__ )
#define INSTRSET 7
#elif defined ( __SSE4_2__ )
#define INSTRSET 6
#elif defined ( __SSE4_1__ )
#define INSTRSET 5
#elif defined ( __SSSE3__ )
#define INSTRSET 4
#elif defined ( __SSE3__ )
#define INSTRSET 3
#elif defined ( __SSE2__ ) || defined ( __x86_64__ )
#define INSTRSET 2
#elif defined ( __SSE__ )
#define INSTRSET 1
#elif defined ( _M_IX86_FP )           // Defined in MS compiler. 1: SSE, 2: SSE2
#define INSTRSET _M_IX86_FP
#else
#define INSTRSET 0
#endif // instruction set defines
#endif // INSTRSET


#if INSTRSET >= 8 && !defined(__FMA__)
// Assume that all processors that have AVX2 also have FMA3
#if defined (__GNUC__) && ! defined (__INTEL_COMPILER)
// Prevent error message in g++ and Clang when using FMA intrinsics with avx2:
#if !defined(DISABLE_WARNING_AVX2_WITHOUT_FMA)
#pragma message "It is recommended to specify also option -mfma when using -mavx2 or higher"
#endif
#elif ! defined (__clang__)
#define __FMA__  1
#endif
#endif

// Header files for non-vector intrinsic functions including _BitScanReverse(int), __cpuid(int[4],int), _xgetbv(int)
#ifdef _MSC_VER                        // Microsoft compiler or compatible Intel compiler
#include <intrin.h>
#pragma warning(disable: 6323 4514 4710 4711) // Diasble annoying warnings
#else
#include <x86intrin.h>                 // Gcc or Clang compiler
#endif

#include <stdint.h>                    // Define integer types with known size
#include <limits.h>                    // Define INT_MAX
#include <stdlib.h>                    // define abs(int)

// GCC version
#if defined(__GNUC__) && !defined (GCC_VERSION) && !defined (__clang__)
#define GCC_VERSION  ((__GNUC__) * 10000 + (__GNUC_MINOR__) * 100 + (__GNUC_PATCHLEVEL__))
#endif

// Clang version
#if defined (__clang__)
#define CLANG_VERSION  ((__clang_major__) * 10000 + (__clang_minor__) * 100 + (__clang_patchlevel__))
// Problem: The version number is not consistent across platforms
// http://llvm.org/bugs/show_bug.cgi?id=12643
// Apple bug 18746972
#endif

// Fix problem with non-overloadable macros named min and max in WinDef.h
#ifdef _MSC_VER
#if defined (_WINDEF_) && defined(min) && defined(max)
#undef min
#undef max
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

// warning for poor support for AVX512F in MS compiler
#if !defined(__INTEL_COMPILER) && !defined(__clang__)
#if INSTRSET == 9
#pragma message("Warning: MS compiler cannot generate code for AVX512F without AVX512DQ")
#endif
#if _MSC_VER < 1920 && INSTRSET > 8
#pragma message("Warning: Your compiler has poor support for AVX512. Code may be erroneous.\nPlease use a newer compiler version or a different compiler!")
#endif
#endif // __INTEL_COMPILER
#endif // _MSC_VER

#if defined(__INTEL_COMPILER) && __INTEL_COMPILER < 2021
#error The Intel compiler version 19.00 cannot compile VCL version 2
#endif

/* Clang problem:
The Clang compiler treats the intrinsic vector types __m128, __m128i, and __m128d as identical.
See the bug report at https://bugs.llvm.org/show_bug.cgi?id=17164
Additional problem: The version number is not consistent across platforms. The Apple build has
different version numbers. We have to rely on __apple_build_version__ on the Mac platform:
http://llvm.org/bugs/show_bug.cgi?id=12643
We have to make switches here when - hopefully - the error some day has been fixed.
We need different version checks with and whithout __apple_build_version__
*/
#if (defined (__clang__) || defined(__apple_build_version__)) && !defined(__INTEL_COMPILER)
#define FIX_CLANG_VECTOR_ALIAS_AMBIGUITY
#endif

#if defined (__GNUC__) && __GNUC__ < 10 && !defined(__clang__)
// Gcc 9 and earlier donot have simde_mm256_zextsi128_si256 and similar functions for xero-extending vector registers
#define ZEXT_MISSING
#endif

#endif // INSTRSET_H
