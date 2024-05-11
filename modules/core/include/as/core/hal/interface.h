#ifndef OPENAS_CORE_HAL_INTERFACE_H
#define OPENAS_CORE_HAL_INTERFACE_H

//! @addtogroup core_hal_interface
//! @{

//! @name Return codes
//! @{
#define AS_HAL_ERROR_OK 0
#define AS_HAL_ERROR_NOT_IMPLEMENTED 1
#define AS_HAL_ERROR_UNKNOWN -1
//! @}

#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#include <stdbool.h>
#endif

//! @name Data types
//! primitive types
//! - schar  - signed 1 byte integer
//! - uchar  - unsigned 1 byte integer
//! - short  - signed 2 byte integer
//! - ushort - unsigned 2 byte integer
//! - int    - signed 4 byte integer
//! - uint   - unsigned 4 byte integer
//! - int64  - signed 8 byte integer
//! - uint64 - unsigned 8 byte integer
//! @{
#if !defined _MSC_VER && !defined __BORLANDC__
#  if defined __cplusplus && __cplusplus >= 201103L && !defined __APPLE__
#    include <cstdint>
#    ifdef __NEWLIB__
        typedef unsigned int uint;
#    else
        typedef std::uint32_t uint;
#    endif
#  else
#    include <stdint.h>
     typedef uint32_t uint;
#  endif
#else
   typedef unsigned uint;
#endif

typedef signed char schar;

#ifndef __IPL_H__
   typedef unsigned char uchar;
   typedef unsigned short ushort;
#endif

#if defined _MSC_VER || defined __BORLANDC__
   typedef __int64 int64;
   typedef unsigned __int64 uint64;
#  define AS_BIG_INT(n)   n##I64
#  define AS_BIG_UINT(n)  n##UI64
#else
   typedef int64_t int64;
   typedef uint64_t uint64;
#  define AS_BIG_INT(n)   n##LL
#  define AS_BIG_UINT(n)  n##ULL
#endif

#define AS_USRTYPE1 (void)"AS_USRTYPE1 support has been dropped in OpenAS 4.0"

#define AS_CN_MAX     512
#define AS_CN_SHIFT   3
#define AS_DEPTH_MAX  (1 << AS_CN_SHIFT)

#define AS_8U   0
#define AS_8S   1
#define AS_16U  2
#define AS_16S  3
#define AS_32S  4
#define AS_32F  5
#define AS_64F  6
#define AS_16F  7

#define AS_MAT_DEPTH_MASK       (AS_DEPTH_MAX - 1)
#define AS_MAT_DEPTH(flags)     ((flags) & AS_MAT_DEPTH_MASK)

#define AS_MAKETYPE(depth,cn) (AS_MAT_DEPTH(depth) + (((cn)-1) << AS_CN_SHIFT))
#define AS_MAKE_TYPE AS_MAKETYPE

#define AS_8UC1 AS_MAKETYPE(AS_8U,1)
#define AS_8UC2 AS_MAKETYPE(AS_8U,2)
#define AS_8UC3 AS_MAKETYPE(AS_8U,3)
#define AS_8UC4 AS_MAKETYPE(AS_8U,4)
#define AS_8UC(n) AS_MAKETYPE(AS_8U,(n))

#define AS_8SC1 AS_MAKETYPE(AS_8S,1)
#define AS_8SC2 AS_MAKETYPE(AS_8S,2)
#define AS_8SC3 AS_MAKETYPE(AS_8S,3)
#define AS_8SC4 AS_MAKETYPE(AS_8S,4)
#define AS_8SC(n) AS_MAKETYPE(AS_8S,(n))

#define AS_16UC1 AS_MAKETYPE(AS_16U,1)
#define AS_16UC2 AS_MAKETYPE(AS_16U,2)
#define AS_16UC3 AS_MAKETYPE(AS_16U,3)
#define AS_16UC4 AS_MAKETYPE(AS_16U,4)
#define AS_16UC(n) AS_MAKETYPE(AS_16U,(n))

#define AS_16SC1 AS_MAKETYPE(AS_16S,1)
#define AS_16SC2 AS_MAKETYPE(AS_16S,2)
#define AS_16SC3 AS_MAKETYPE(AS_16S,3)
#define AS_16SC4 AS_MAKETYPE(AS_16S,4)
#define AS_16SC(n) AS_MAKETYPE(AS_16S,(n))

#define AS_32SC1 AS_MAKETYPE(AS_32S,1)
#define AS_32SC2 AS_MAKETYPE(AS_32S,2)
#define AS_32SC3 AS_MAKETYPE(AS_32S,3)
#define AS_32SC4 AS_MAKETYPE(AS_32S,4)
#define AS_32SC(n) AS_MAKETYPE(AS_32S,(n))

#define AS_32FC1 AS_MAKETYPE(AS_32F,1)
#define AS_32FC2 AS_MAKETYPE(AS_32F,2)
#define AS_32FC3 AS_MAKETYPE(AS_32F,3)
#define AS_32FC4 AS_MAKETYPE(AS_32F,4)
#define AS_32FC(n) AS_MAKETYPE(AS_32F,(n))

#define AS_64FC1 AS_MAKETYPE(AS_64F,1)
#define AS_64FC2 AS_MAKETYPE(AS_64F,2)
#define AS_64FC3 AS_MAKETYPE(AS_64F,3)
#define AS_64FC4 AS_MAKETYPE(AS_64F,4)
#define AS_64FC(n) AS_MAKETYPE(AS_64F,(n))

#define AS_16FC1 AS_MAKETYPE(AS_16F,1)
#define AS_16FC2 AS_MAKETYPE(AS_16F,2)
#define AS_16FC3 AS_MAKETYPE(AS_16F,3)
#define AS_16FC4 AS_MAKETYPE(AS_16F,4)
#define AS_16FC(n) AS_MAKETYPE(AS_16F,(n))
//! @}

//! @name Comparison operation
//! @sa cv::CmpTypes
//! @{
#define AS_HAL_CMP_EQ 0
#define AS_HAL_CMP_GT 1
#define AS_HAL_CMP_GE 2
#define AS_HAL_CMP_LT 3
#define AS_HAL_CMP_LE 4
#define AS_HAL_CMP_NE 5
//! @}

//! @name Border processing modes
//! @sa cv::BorderTypes
//! @{
#define AS_HAL_BORDER_CONSTANT 0
#define AS_HAL_BORDER_REPLICATE 1
#define AS_HAL_BORDER_REFLECT 2
#define AS_HAL_BORDER_WRAP 3
#define AS_HAL_BORDER_REFLECT_101 4
#define AS_HAL_BORDER_TRANSPARENT 5
#define AS_HAL_BORDER_ISOLATED 16
//! @}

//! @name DFT flags
//! @{
#define AS_HAL_DFT_INVERSE        1
#define AS_HAL_DFT_SCALE          2
#define AS_HAL_DFT_ROWS           4
#define AS_HAL_DFT_COMPLEX_OUTPUT 16
#define AS_HAL_DFT_REAL_OUTPUT    32
#define AS_HAL_DFT_TWO_STAGE      64
#define AS_HAL_DFT_STAGE_COLS    128
#define AS_HAL_DFT_IS_CONTINUOUS 512
#define AS_HAL_DFT_IS_INPLACE 1024
//! @}

//! @name SVD flags
//! @{
#define AS_HAL_SVD_NO_UV    1
#define AS_HAL_SVD_SHORT_UV 2
#define AS_HAL_SVD_MODIFY_A 4
#define AS_HAL_SVD_FULL_UV  8
//! @}

//! @name Gemm flags
//! @{
#define AS_HAL_GEMM_1_T 1
#define AS_HAL_GEMM_2_T 2
#define AS_HAL_GEMM_3_T 4
//! @}

//! @}

#endif
