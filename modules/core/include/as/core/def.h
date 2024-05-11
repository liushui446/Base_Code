/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Copyright (C) 2013, OpenAS Foundation, all rights reserved.
// Copyright (C) 2015, Itseez Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef AS_CORE_ASDEF_H
#define AS_CORE_ASDEF_H

#include "as/core/version.hpp"

//! @addtogroup core_utils
//! @{

#ifdef AS_INCLUDE_PORT_FILE  // User-provided header file with custom platform configuration
#include AS_INCLUDE_PORT_FILE
#endif

#if !defined AS_DOXYGEN && !defined AS_IGNORE_DEBUG_BUILD_GUARD
#if (defined(_MSC_VER) && (defined(DEBUG) || defined(_DEBUG))) || \
    (defined(_GLIBCXX_DEBUG) || defined(_GLIBCXX_DEBUG_PEDANTIC))
// Guard to prevent using of binary incompatible binaries / runtimes
// https://github.com/openas/openas/pull/9161
#define AS__DEBUG_NS_BEGIN namespace debug_build_guard {
#define AS__DEBUG_NS_END }
namespace as { namespace debug_build_guard { } using namespace debug_build_guard; }
#endif
#endif

#ifndef AS__DEBUG_NS_BEGIN
#define AS__DEBUG_NS_BEGIN
#define AS__DEBUG_NS_END
#endif


#ifdef __AS_BUILD
#include "asconfig.h"
#endif

#ifndef __AS_EXPAND
#define __AS_EXPAND(x) x
#endif

#ifndef __AS_CAT
#define __AS_CAT__(x, y) x ## y
#define __AS_CAT_(x, y) __AS_CAT__(x, y)
#define __AS_CAT(x, y) __AS_CAT_(x, y)
#endif

#define __AS_VA_NUM_ARGS_HELPER(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define __AS_VA_NUM_ARGS(...) __AS_EXPAND(__AS_VA_NUM_ARGS_HELPER(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#ifdef AS_Func
// keep current value (through OpenAS port file)
#elif defined __GNUC__ || (defined (__cpluscplus) && (__cpluscplus >= 201103))
#define AS_Func __func__
#elif defined __clang__ && (__clang_minor__ * 100 + __clang_major__ >= 305)
#define AS_Func __func__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION >= 199901)
#define AS_Func __func__
#elif defined _MSC_VER
#define AS_Func __FUNCTION__
#elif defined(__INTEL_COMPILER) && (_INTEL_COMPILER >= 600)
#define AS_Func __FUNCTION__
#elif defined __IBMCPP__ && __IBMCPP__ >=500
#define AS_Func __FUNCTION__
#elif defined __BORLAND__ && (__BORLANDC__ >= 0x550)
#define AS_Func __FUNC__
#else
#define AS_Func "<unknown>"
#endif

//! @cond IGNORED

//////////////// static assert /////////////////
#define ASAUX_CONCAT_EXP(a, b) a##b
#define ASAUX_CONCAT(a, b) ASAUX_CONCAT_EXP(a,b)

#if defined(__clang__)
#  ifndef __has_extension
#    define __has_extension __has_feature /* compatibility, for older versions of clang */
#  endif
#  if __has_extension(cxx_static_assert)
#    define AS_StaticAssert(condition, reason)    static_assert((condition), reason " " #condition)
#  elif __has_extension(c_static_assert)
#    define AS_StaticAssert(condition, reason)    _Static_assert((condition), reason " " #condition)
#  endif
#elif defined(__GNUC__)
#  if (defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L)
#    define AS_StaticAssert(condition, reason)    static_assert((condition), reason " " #condition)
#  endif
#elif defined(_MSC_VER)
#  if _MSC_VER >= 1600 /* MSVC 10 */
#    define AS_StaticAssert(condition, reason)    static_assert((condition), reason " " #condition)
#  endif
#endif
#ifndef AS_StaticAssert
#  if !defined(__clang__) && defined(__GNUC__) && (__GNUC__*100 + __GNUC_MINOR__ > 302)
#    define AS_StaticAssert(condition, reason) ({ extern int __attribute__((error("AS_StaticAssert: " reason " " #condition))) AS_StaticAssert(); ((condition) ? 0 : AS_StaticAssert()); })
#  else
namespace as {
     template <bool x> struct AS_StaticAssert_failed;
     template <> struct AS_StaticAssert_failed<true> { enum { val = 1 }; };
     template<int x> struct AS_StaticAssert_test {};
}
#    define AS_StaticAssert(condition, reason)\
       typedef as::AS_StaticAssert_test< sizeof(as::AS_StaticAssert_failed< static_cast<bool>(condition) >) > ASAUX_CONCAT(AS_StaticAssert_failed_at_, __LINE__)
#  endif
#endif

// Suppress warning "-Wdeprecated-declarations" / C4996
#if defined(_MSC_VER)
    #define AS_DO_PRAGMA(x) __pragma(x)
#elif defined(__GNUC__)
    #define AS_DO_PRAGMA(x) _Pragma (#x)
#else
    #define AS_DO_PRAGMA(x)
#endif

#ifdef _MSC_VER
#define AS_SUPPRESS_DEPRECATED_START \
    AS_DO_PRAGMA(warning(push)) \
    AS_DO_PRAGMA(warning(disable: 4996))
#define AS_SUPPRESS_DEPRECATED_END AS_DO_PRAGMA(warning(pop))
#elif defined (__clang__) || ((__GNUC__)  && (__GNUC__*100 + __GNUC_MINOR__ > 405))
#define AS_SUPPRESS_DEPRECATED_START \
    AS_DO_PRAGMA(GCC diagnostic push) \
    AS_DO_PRAGMA(GCC diagnostic ignored "-Wdeprecated-declarations")
#define AS_SUPPRESS_DEPRECATED_END AS_DO_PRAGMA(GCC diagnostic pop)
#else
#define AS_SUPPRESS_DEPRECATED_START
#define AS_SUPPRESS_DEPRECATED_END
#endif

#define AS_UNUSED(name) (void)name

//! @endcond

// undef problematic defines sometimes defined by system headers (windows.h in particular)
#undef asall
#undef min
#undef max
#undef abs
#undef Complex

#if defined __cplusplus
#include <limits>
#else
#include <limits.h>
#endif

#include "as/core/hal/interface.h"

#if defined __ICL
#  define AS_ICC   __ICL
#elif defined __ICC
#  define AS_ICC   __ICC
#elif defined __ECL
#  define AS_ICC   __ECL
#elif defined __ECC
#  define AS_ICC   __ECC
#elif defined __INTEL_COMPILER
#  define AS_ICC   __INTEL_COMPILER
#endif

#ifndef AS_INLINE
#  if defined __cplusplus
#    define AS_INLINE static inline
#  elif defined _MSC_VER
#    define AS_INLINE __inline
#  else
#    define AS_INLINE static
#  endif
#endif

#ifndef AS_ALWAYS_INLINE
#if defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#define AS_ALWAYS_INLINE inline __attribute__((always_inline))
#elif defined(_MSC_VER)
#define AS_ALWAYS_INLINE __forceinline
#else
#define AS_ALWAYS_INLINE inline
#endif
#endif

#if defined AS_DISABLE_OPTIMIZATION || (defined AS_ICC && !defined AS_ENABLE_UNROLLED)
#  define AS_ENABLE_UNROLLED 0
#else
#  define AS_ENABLE_UNROLLED 1
#endif

#ifdef __GNUC__
#  define AS_DECL_ALIGNED(x) __attribute__ ((aligned (x)))
#elif defined _MSC_VER
#  define AS_DECL_ALIGNED(x) __declspec(align(x))
#else
#  define AS_DECL_ALIGNED(x)
#endif

/* CPU features and intrinsics support */
#define AS_CPU_NONE             0
#define AS_CPU_MMX              1
#define AS_CPU_SSE              2
#define AS_CPU_SSE2             3
#define AS_CPU_SSE3             4
#define AS_CPU_SSSE3            5
#define AS_CPU_SSE4_1           6
#define AS_CPU_SSE4_2           7
#define AS_CPU_POPCNT           8
#define AS_CPU_FP16             9
#define AS_CPU_AVX              10
#define AS_CPU_AVX2             11
#define AS_CPU_FMA3             12

#define AS_CPU_AVX_512F         13
#define AS_CPU_AVX_512BW        14
#define AS_CPU_AVX_512CD        15
#define AS_CPU_AVX_512DQ        16
#define AS_CPU_AVX_512ER        17
#define AS_CPU_AVX_512IFMA512   18 // deprecated
#define AS_CPU_AVX_512IFMA      18
#define AS_CPU_AVX_512PF        19
#define AS_CPU_AVX_512VBMI      20
#define AS_CPU_AVX_512VL        21
#define AS_CPU_AVX_512VBMI2     22
#define AS_CPU_AVX_512VNNI      23
#define AS_CPU_AVX_512BITALG    24
#define AS_CPU_AVX_512VPOPCNTDQ 25
#define AS_CPU_AVX_5124VNNIW    26
#define AS_CPU_AVX_5124FMAPS    27

#define AS_CPU_NEON             100

#define AS_CPU_MSA              150

#define AS_CPU_VSX              200
#define AS_CPU_VSX3             201

#define AS_CPU_RVV              210

// CPU features groups
#define AS_CPU_AVX512_SKX       256
#define AS_CPU_AVX512_COMMON    257
#define AS_CPU_AVX512_KNL       258
#define AS_CPU_AVX512_KNM       259
#define AS_CPU_AVX512_CNL       260
#define AS_CPU_AVX512_CLX       261
#define AS_CPU_AVX512_ICL       262

// when adding to this list remember to update the following enum
#define AS_HARDWARE_MAX_FEATURE 512
#if 0
/** @brief Available CPU features.
*/
enum CpuFeatures {
    CPU_MMX             = 1,
    CPU_SSE             = 2,
    CPU_SSE2            = 3,
    CPU_SSE3            = 4,
    CPU_SSSE3           = 5,
    CPU_SSE4_1          = 6,
    CPU_SSE4_2          = 7,
    CPU_POPCNT          = 8,
    CPU_FP16            = 9,
    CPU_AVX             = 10,
    CPU_AVX2            = 11,
    CPU_FMA3            = 12,

    CPU_AVX_512F        = 13,
    CPU_AVX_512BW       = 14,
    CPU_AVX_512CD       = 15,
    CPU_AVX_512DQ       = 16,
    CPU_AVX_512ER       = 17,
    CPU_AVX_512IFMA512  = 18, // deprecated
    CPU_AVX_512IFMA     = 18,
    CPU_AVX_512PF       = 19,
    CPU_AVX_512VBMI     = 20,
    CPU_AVX_512VL       = 21,
    CPU_AVX_512VBMI2    = 22,
    CPU_AVX_512VNNI     = 23,
    CPU_AVX_512BITALG   = 24,
    CPU_AVX_512VPOPCNTDQ= 25,
    CPU_AVX_5124VNNIW   = 26,
    CPU_AVX_5124FMAPS   = 27,

    CPU_NEON            = 100,

    CPU_MSA             = 150,

    CPU_VSX             = 200,
    CPU_VSX3            = 201,

    CPU_RVV             = 210,

    CPU_AVX512_SKX      = 256, //!< Skylake-X with AVX-512F/CD/BW/DQ/VL
    CPU_AVX512_COMMON   = 257, //!< Common instructions AVX-512F/CD for all CPUs that support AVX-512
    CPU_AVX512_KNL      = 258, //!< Knights Landing with AVX-512F/CD/ER/PF
    CPU_AVX512_KNM      = 259, //!< Knights Mill with AVX-512F/CD/ER/PF/4FMAPS/4VNNIW/VPOPCNTDQ
    CPU_AVX512_CNL      = 260, //!< Cannon Lake with AVX-512F/CD/BW/DQ/VL/IFMA/VBMI
    CPU_AVX512_CLX      = 261, //!< Cascade Lake with AVX-512F/CD/BW/DQ/VL/VNNI
    CPU_AVX512_ICL      = 262, //!< Ice Lake with AVX-512F/CD/BW/DQ/VL/IFMA/VBMI/VNNI/VBMI2/BITALG/VPOPCNTDQ

    CPU_MAX_FEATURE     = 512  // see AS_HARDWARE_MAX_FEATURE
};

#endif
//#include "as_cpu_dispatch.h"

#if !defined(AS_STRONG_ALIGNMENT) && defined(__arm__) && !(defined(__aarch64__) || defined(_M_ARM64))
// int*, int64* should be propertly aligned pointers on ARMv7
#define AS_STRONG_ALIGNMENT 1
#endif
#if !defined(AS_STRONG_ALIGNMENT)
#define AS_STRONG_ALIGNMENT 0
#endif

/* fundamental constants */
#define AS_PI   3.1415926535897932384626433832795
#define AS_2PI  6.283185307179586476925286766559
#define AS_LOG2 0.69314718055994530941723212145818

#if defined __ARM_FP16_FORMAT_IEEE \
    && !defined __CUDACC__
#  define AS_FP16_TYPE 1
#else
#  define AS_FP16_TYPE 0
#endif

typedef union as16suf
{
    short i;
    ushort u;
#if AS_FP16_TYPE
    __fp16 h;
#endif
}
as16suf;

typedef union as32suf
{
    int i;
    unsigned u;
    float f;
}
as32suf;

typedef union as64suf
{
    int64 i;
    uint64 u;
    double f;
}
as64suf;

#ifndef AS_ABI_COMPATIBILITY
#define AS_ABI_COMPATIBILITY 400
#endif

#ifdef __AS_BUILD
#  define DISABLE_AS_3_COMPATIBILITY
#  define AS_DISABLE_DEPRECATED_COMPATIBILITY
#endif

#ifndef AS_EXPORTS
# if (defined _WIN32 || defined WINCE || defined __CYGWIN__) && defined(ASAPI_EXPORTS)
#   define AS_EXPORTS __declspec(dllexport)
# elif defined __GNUC__ && __GNUC__ >= 4 && (defined(ASAPI_EXPORTS) || defined(__APPLE__))
#   define AS_EXPORTS __attribute__ ((visibility ("default")))
# endif
#endif

#ifndef AS_EXPORTS
#define AS_EXPORTS __declspec(dllexport)
#endif

#ifdef _MSC_VER
#   define AS_EXPORTS_TEMPLATE
#else
#   define AS_EXPORTS_TEMPLATE AS_EXPORTS
#endif

#ifndef AS_DEPRECATED
#  if defined(__GNUC__)
#    define AS_DEPRECATED __attribute__ ((deprecated))
#  elif defined(_MSC_VER)
#    define AS_DEPRECATED __declspec(deprecated)
#  else
#    define AS_DEPRECATED
#  endif
#endif

#ifndef AS_DEPRECATED_EXTERNAL
#  if defined(__AS_BUILD)
#    define AS_DEPRECATED_EXTERNAL /* nothing */
#  else
#    define AS_DEPRECATED_EXTERNAL AS_DEPRECATED
#  endif
#endif


#ifndef AS_EXTERN_C
#  ifdef __cplusplus
#    define AS_EXTERN_C extern "C"
#  else
#    define AS_EXTERN_C
#  endif
#endif

/* special informative macros for wrapper generators */
#define AS_EXPORTS_W AS_EXPORTS
#define AS_EXPORTS_W_SIMPLE AS_EXPORTS
#define AS_EXPORTS_AS(synonym) AS_EXPORTS
#define AS_EXPORTS_W_MAP AS_EXPORTS
#define AS_IN_OUT
#define AS_OUT
#define AS_PROP
#define AS_PROP_RW
#define AS_WRAP
#define AS_WRAP_AS(synonym)
#define AS_WRAP_MAPPABLE(mappable)
#define AS_WRAP_PHANTOM(phantom_header)
#define AS_WRAP_DEFAULT(val)

/****************************************************************************************\
*                                  Matrix type (Mat)                                     *
\****************************************************************************************/

#define AS_MAT_CN_MASK          ((AS_CN_MAX - 1) << AS_CN_SHIFT)
#define AS_MAT_CN(flags)        ((((flags) & AS_MAT_CN_MASK) >> AS_CN_SHIFT) + 1)
#define AS_MAT_TYPE_MASK        (AS_DEPTH_MAX*AS_CN_MAX - 1)
#define AS_MAT_TYPE(flags)      ((flags) & AS_MAT_TYPE_MASK)
#define AS_MAT_CONT_FLAG_SHIFT  14
#define AS_MAT_CONT_FLAG        (1 << AS_MAT_CONT_FLAG_SHIFT)
#define AS_IS_MAT_CONT(flags)   ((flags) & AS_MAT_CONT_FLAG)
#define AS_IS_CONT_MAT          AS_IS_MAT_CONT
#define AS_SUBMAT_FLAG_SHIFT    15
#define AS_SUBMAT_FLAG          (1 << AS_SUBMAT_FLAG_SHIFT)
#define AS_IS_SUBMAT(flags)     ((flags) & AS_MAT_SUBMAT_FLAG)

/** Size of each channel item,
   0x28442211 = 0010 1000 0100 0100 0010 0010 0001 0001 ~ array of sizeof(arr_type_elem) */
#define AS_ELEM_SIZE1(type) ((0x28442211 >> AS_MAT_DEPTH(type)*4) & 15)

#define AS_ELEM_SIZE(type) (AS_MAT_CN(type)*AS_ELEM_SIZE1(type))

#ifndef MIN
#  define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#  define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif

///////////////////////////////////////// Enum operators ///////////////////////////////////////

/**

Provides compatibility operators for both classical and C++11 enum classes,
as well as exposing the C++11 enum class members for backwards compatibility

@code
    // Provides operators required for flag enums
    AS_ENUM_FLAGS(AccessFlag)

    // Exposes the listed members of the enum class AccessFlag to the current namespace
    AS_ENUM_CLASS_EXPOSE(AccessFlag, ACCESS_READ [, ACCESS_WRITE [, ...] ]);
@endcode
*/

#define __AS_ENUM_CLASS_EXPOSE_1(EnumType, MEMBER_CONST)                                              \
static const EnumType MEMBER_CONST = EnumType::MEMBER_CONST;                                          \

#define __AS_ENUM_CLASS_EXPOSE_2(EnumType, MEMBER_CONST, ...)                                         \
__AS_ENUM_CLASS_EXPOSE_1(EnumType, MEMBER_CONST);                                                     \
__AS_EXPAND(__AS_ENUM_CLASS_EXPOSE_1(EnumType, __VA_ARGS__));                                         \

#define __AS_ENUM_CLASS_EXPOSE_3(EnumType, MEMBER_CONST, ...)                                         \
__AS_ENUM_CLASS_EXPOSE_1(EnumType, MEMBER_CONST);                                                     \
__AS_EXPAND(__AS_ENUM_CLASS_EXPOSE_2(EnumType, __VA_ARGS__));                                         \

#define __AS_ENUM_CLASS_EXPOSE_4(EnumType, MEMBER_CONST, ...)                                         \
__AS_ENUM_CLASS_EXPOSE_1(EnumType, MEMBER_CONST);                                                     \
__AS_EXPAND(__AS_ENUM_CLASS_EXPOSE_3(EnumType, __VA_ARGS__));                                         \

#define __AS_ENUM_CLASS_EXPOSE_5(EnumType, MEMBER_CONST, ...)                                         \
__AS_ENUM_CLASS_EXPOSE_1(EnumType, MEMBER_CONST);                                                     \
__AS_EXPAND(__AS_ENUM_CLASS_EXPOSE_4(EnumType, __VA_ARGS__));                                         \

#define __AS_ENUM_CLASS_EXPOSE_6(EnumType, MEMBER_CONST, ...)                                         \
__AS_ENUM_CLASS_EXPOSE_1(EnumType, MEMBER_CONST);                                                     \
__AS_EXPAND(__AS_ENUM_CLASS_EXPOSE_5(EnumType, __VA_ARGS__));                                         \

#define __AS_ENUM_CLASS_EXPOSE_7(EnumType, MEMBER_CONST, ...)                                         \
__AS_ENUM_CLASS_EXPOSE_1(EnumType, MEMBER_CONST);                                                     \
__AS_EXPAND(__AS_ENUM_CLASS_EXPOSE_6(EnumType, __VA_ARGS__));                                         \

#define __AS_ENUM_CLASS_EXPOSE_8(EnumType, MEMBER_CONST, ...)                                         \
__AS_ENUM_CLASS_EXPOSE_1(EnumType, MEMBER_CONST);                                                     \
__AS_EXPAND(__AS_ENUM_CLASS_EXPOSE_7(EnumType, __VA_ARGS__));                                         \

#define __AS_ENUM_CLASS_EXPOSE_9(EnumType, MEMBER_CONST, ...)                                         \
__AS_ENUM_CLASS_EXPOSE_1(EnumType, MEMBER_CONST);                                                     \
__AS_EXPAND(__AS_ENUM_CLASS_EXPOSE_8(EnumType, __VA_ARGS__));                                         \

#define __AS_ENUM_FLAGS_LOGICAL_NOT(EnumType)                                                         \
static inline bool operator!(const EnumType& val)                                                     \
{                                                                                                     \
    typedef std::underlying_type<EnumType>::type UnderlyingType;                                      \
    return !static_cast<UnderlyingType>(val);                                                         \
}                                                                                                     \

#define __AS_ENUM_FLAGS_LOGICAL_NOT_EQ(Arg1Type, Arg2Type)                                            \
static inline bool operator!=(const Arg1Type& a, const Arg2Type& b)                                   \
{                                                                                                     \
    return static_cast<int>(a) != static_cast<int>(b);                                                \
}                                                                                                     \

#define __AS_ENUM_FLAGS_LOGICAL_EQ(Arg1Type, Arg2Type)                                                \
static inline bool operator==(const Arg1Type& a, const Arg2Type& b)                                   \
{                                                                                                     \
    return static_cast<int>(a) == static_cast<int>(b);                                                \
}                                                                                                     \

#define __AS_ENUM_FLAGS_BITWISE_NOT(EnumType)                                                         \
static inline EnumType operator~(const EnumType& val)                                                 \
{                                                                                                     \
    typedef std::underlying_type<EnumType>::type UnderlyingType;                                      \
    return static_cast<EnumType>(~static_cast<UnderlyingType>(val));                                  \
}                                                                                                     \

#define __AS_ENUM_FLAGS_BITWISE_OR(EnumType, Arg1Type, Arg2Type)                                      \
static inline EnumType operator|(const Arg1Type& a, const Arg2Type& b)                                \
{                                                                                                     \
    typedef std::underlying_type<EnumType>::type UnderlyingType;                                      \
    return static_cast<EnumType>(static_cast<UnderlyingType>(a) | static_cast<UnderlyingType>(b));    \
}                                                                                                     \

#define __AS_ENUM_FLAGS_BITWISE_AND(EnumType, Arg1Type, Arg2Type)                                     \
static inline EnumType operator&(const Arg1Type& a, const Arg2Type& b)                                \
{                                                                                                     \
    typedef std::underlying_type<EnumType>::type UnderlyingType;                                      \
    return static_cast<EnumType>(static_cast<UnderlyingType>(a) & static_cast<UnderlyingType>(b));    \
}                                                                                                     \

#define __AS_ENUM_FLAGS_BITWISE_XOR(EnumType, Arg1Type, Arg2Type)                                     \
static inline EnumType operator^(const Arg1Type& a, const Arg2Type& b)                                \
{                                                                                                     \
    typedef std::underlying_type<EnumType>::type UnderlyingType;                                      \
    return static_cast<EnumType>(static_cast<UnderlyingType>(a) ^ static_cast<UnderlyingType>(b));    \
}                                                                                                     \

#define __AS_ENUM_FLAGS_BITWISE_OR_EQ(EnumType, Arg1Type)                                             \
static inline EnumType& operator|=(EnumType& _this, const Arg1Type& val)                              \
{                                                                                                     \
    _this = static_cast<EnumType>(static_cast<int>(_this) | static_cast<int>(val));                   \
    return _this;                                                                                     \
}                                                                                                     \

#define __AS_ENUM_FLAGS_BITWISE_AND_EQ(EnumType, Arg1Type)                                            \
static inline EnumType& operator&=(EnumType& _this, const Arg1Type& val)                              \
{                                                                                                     \
    _this = static_cast<EnumType>(static_cast<int>(_this) & static_cast<int>(val));                   \
    return _this;                                                                                     \
}                                                                                                     \

#define __AS_ENUM_FLAGS_BITWISE_XOR_EQ(EnumType, Arg1Type)                                            \
static inline EnumType& operator^=(EnumType& _this, const Arg1Type& val)                              \
{                                                                                                     \
    _this = static_cast<EnumType>(static_cast<int>(_this) ^ static_cast<int>(val));                   \
    return _this;                                                                                     \
}                                                                                                     \

#define AS_ENUM_CLASS_EXPOSE(EnumType, ...)                                                           \
__AS_EXPAND(__AS_CAT(__AS_ENUM_CLASS_EXPOSE_, __AS_VA_NUM_ARGS(__VA_ARGS__))(EnumType, __VA_ARGS__)); \

#define AS_ENUM_FLAGS(EnumType)                                                                       \
__AS_ENUM_FLAGS_LOGICAL_NOT      (EnumType)                                                           \
__AS_ENUM_FLAGS_LOGICAL_EQ       (EnumType, int)                                                      \
__AS_ENUM_FLAGS_LOGICAL_NOT_EQ   (EnumType, int)                                                      \
                                                                                                      \
__AS_ENUM_FLAGS_BITWISE_NOT      (EnumType)                                                           \
__AS_ENUM_FLAGS_BITWISE_OR       (EnumType, EnumType, EnumType)                                       \
__AS_ENUM_FLAGS_BITWISE_AND      (EnumType, EnumType, EnumType)                                       \
__AS_ENUM_FLAGS_BITWISE_XOR      (EnumType, EnumType, EnumType)                                       \
                                                                                                      \
__AS_ENUM_FLAGS_BITWISE_OR_EQ    (EnumType, EnumType)                                                 \
__AS_ENUM_FLAGS_BITWISE_AND_EQ   (EnumType, EnumType)                                                 \
__AS_ENUM_FLAGS_BITWISE_XOR_EQ   (EnumType, EnumType)                                                 \

/****************************************************************************************\
*                                    static analysys                                     *
\****************************************************************************************/

// In practice, some macro are not processed correctly (noreturn is not detected).
// We need to use simplified definition for them.
#ifndef AS_STATIC_ANALYSIS
# if defined(__KLOCWORK__) || defined(__clang_analyzer__) || defined(__COVERITY__)
#   define AS_STATIC_ANALYSIS 1
# endif
#else
# if defined(AS_STATIC_ANALYSIS) && !(__AS_CAT(1, AS_STATIC_ANALYSIS) == 1)  // defined and not empty
#   if 0 == AS_STATIC_ANALYSIS
#     undef AS_STATIC_ANALYSIS
#   endif
# endif
#endif

/****************************************************************************************\
*                                    Thread sanitizer                                    *
\****************************************************************************************/
#ifndef AS_THREAD_SANITIZER
# if defined(__has_feature)
#   if __has_feature(thread_sanitizer)
#     define AS_THREAD_SANITIZER
#   endif
# endif
#endif

/****************************************************************************************\
*          exchange-add operation for atomic operations on reference counters            *
\****************************************************************************************/

#ifdef AS_XADD
  // allow to use user-defined macro
#elif defined __GNUC__ || defined __clang__
#  if defined __clang__ && __clang_major__ >= 3 && !defined __ANDROID__ && !defined __EMSCRIPTEN__ && !defined(__CUDACC__)  && !defined __INTEL_COMPILER
#    ifdef __ATOMIC_ACQ_REL
#      define AS_XADD(addr, delta) __c11_atomic_fetch_add((_Atomic(int)*)(addr), delta, __ATOMIC_ACQ_REL)
#    else
#      define AS_XADD(addr, delta) __atomic_fetch_add((_Atomic(int)*)(addr), delta, 4)
#    endif
#  else
#    if defined __ATOMIC_ACQ_REL && !defined __clang__
       // version for gcc >= 4.7
#      define AS_XADD(addr, delta) (int)__atomic_fetch_add((unsigned*)(addr), (unsigned)(delta), __ATOMIC_ACQ_REL)
#    else
#      define AS_XADD(addr, delta) (int)__sync_fetch_and_add((unsigned*)(addr), (unsigned)(delta))
#    endif
#  endif
#elif defined _MSC_VER && !defined RC_INVOKED
#  include <intrin.h>
#  define AS_XADD(addr, delta) (int)_InterlockedExchangeAdd((long volatile*)addr, delta)
#else
  #ifdef AS_FORCE_UNSAFE_XADD
    AS_INLINE AS_XADD(int* addr, int delta) { int tmp = *addr; *addr += delta; return tmp; }
  #else
    #error "OpenAS: can't define safe AS_XADD macro for current platform (unsupported). Define AS_XADD macro through custom port header (see AS_INCLUDE_PORT_FILE)"
  #endif
#endif


/****************************************************************************************\
*                                  AS_NORETURN attribute                                 *
\****************************************************************************************/

#ifndef AS_NORETURN
#  if defined(__GNUC__)
#    define AS_NORETURN __attribute__((__noreturn__))
#  elif defined(_MSC_VER) && (_MSC_VER >= 1300)
#    define AS_NORETURN __declspec(noreturn)
#  else
#    define AS_NORETURN /* nothing by default */
#  endif
#endif


/****************************************************************************************\
*                                  AS_NODISCARD attribute                                *
* encourages the compiler to issue a warning if the return value is discarded (C++17)    *
\****************************************************************************************/
#ifndef AS_NODISCARD
#  if defined(__GNUC__)
#    define AS_NODISCARD __attribute__((__warn_unused_result__)) // at least available with GCC 3.4
#  elif defined(__clang__) && defined(__has_attribute)
#    if __has_attribute(__warn_unused_result__)
#      define AS_NODISCARD __attribute__((__warn_unused_result__))
#    endif
#  endif
#endif
#ifndef AS_NODISCARD
#  define AS_NODISCARD /* nothing by default */
#endif


/****************************************************************************************\
*                                    C++ 11                                              *
\****************************************************************************************/
#ifndef AS_CXX11
#  if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1800)
#    define AS_CXX11 1
#  endif
#else
#  if AS_CXX11 == 0
#    undef AS_CXX11
#  endif
#endif
#ifndef AS_CXX11
#  error "OpenAS 4.x+ requires enabled C++11 support"
#endif

#define AS_CXX_MOVE_SEMANTICS 1
#define AS_CXX_MOVE(x) std::move(x)
#define AS_CXX_STD_ARRAY 1
#include <array>
#ifndef AS_OVERRIDE
#  define AS_OVERRIDE override
#endif
#ifndef AS_FINAL
#  define AS_FINAL final
#endif

#ifndef AS_NOEXCEPT
#  if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900/*MSVS 2015*/)
#    define AS_NOEXCEPT noexcept
#  endif
#endif
#ifndef AS_NOEXCEPT
#  define AS_NOEXCEPT
#endif

#ifndef AS_CONSTEXPR
#  if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900/*MSVS 2015*/)
#    define AS_CONSTEXPR constexpr
#  endif
#endif
#ifndef AS_CONSTEXPR
#  define AS_CONSTEXPR
#endif

// Integer types portatibility
#ifdef AS_STDINT_HEADER
#include AS_STDINT_HEADER
#elif defined(__cplusplus)
#if defined(_MSC_VER) && _MSC_VER < 1600 /* MSVS 2010 */
namespace as {
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed __int64 int64_t;
typedef unsigned __int64 uint64_t;
}
#elif defined(_MSC_VER) || __cplusplus >= 201103L
#include <cstdint>
namespace as {
using std::int8_t;
using std::uint8_t;
using std::int16_t;
using std::uint16_t;
using std::int32_t;
using std::uint32_t;
using std::int64_t;
using std::uint64_t;
}
#else
#include <stdint.h>
namespace as {
typedef ::int8_t int8_t;
typedef ::uint8_t uint8_t;
typedef ::int16_t int16_t;
typedef ::uint16_t uint16_t;
typedef ::int32_t int32_t;
typedef ::uint32_t uint32_t;
typedef ::int64_t int64_t;
typedef ::uint64_t uint64_t;
}
#endif
#else // pure C
#include <stdint.h>
#endif

#ifdef __cplusplus
namespace as
{

class float16_t
{
public:
#if AS_FP16_TYPE

    float16_t() : h(0) {}
    explicit float16_t(float x) { h = (__fp16)x; }
    operator float() const { return (float)h; }
    static float16_t fromBits(ushort w)
    {
        as16suf u;
        u.u = w;
        float16_t result;
        result.h = u.h;
        return result;
    }
    static float16_t zero()
    {
        float16_t result;
        result.h = (__fp16)0;
        return result;
    }
    ushort bits() const
    {
        as16suf u;
        u.h = h;
        return u.u;
    }
protected:
    __fp16 h;

#else
    float16_t() : w(0) {}
    explicit float16_t(float x)
    {
    #if AS_FP16
        __m128 v = _mm_load_ss(&x);
        w = (ushort)_mm_astsi128_si32(_mm_astps_ph(v, 0));
    #else
        as32suf in;
        in.f = x;
        unsigned sign = in.u & 0x80000000;
        in.u ^= sign;

        if( in.u >= 0x47800000 )
            w = (ushort)(in.u > 0x7f800000 ? 0x7e00 : 0x7c00);
        else
        {
            if (in.u < 0x38800000)
            {
                in.f += 0.5f;
                w = (ushort)(in.u - 0x3f000000);
            }
            else
            {
                unsigned t = in.u + 0xc8000fff;
                w = (ushort)((t + ((in.u >> 13) & 1)) >> 13);
            }
        }

        w = (ushort)(w | (sign >> 16));
    #endif
    }

    operator float() const
    {
    #if AS_FP16
        float f;
        _mm_store_ss(&f, _mm_astph_ps(_mm_astsi32_si128(w)));
        return f;
    #else
        as32suf out;

        unsigned t = ((w & 0x7fff) << 13) + 0x38000000;
        unsigned sign = (w & 0x8000) << 16;
        unsigned e = w & 0x7c00;

        out.u = t + (1 << 23);
        out.u = (e >= 0x7c00 ? t + 0x38000000 :
                 e == 0 ? (static_cast<void>(out.f -= 6.103515625e-05f), out.u) : t) | sign;
        return out.f;
    #endif
    }

    static float16_t fromBits(ushort b)
    {
        float16_t result;
        result.w = b;
        return result;
    }
    static float16_t zero()
    {
        float16_t result;
        result.w = (ushort)0;
        return result;
    }
    ushort bits() const { return w; }
protected:
    ushort w;

#endif
};

}
#endif

//! @}

#ifndef __cplusplus
#include "as/core/fast_math.hpp" // define asRound(double)
#endif
#endif // AS_CORE_ASDEF_H
