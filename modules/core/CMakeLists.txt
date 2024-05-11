set(the_description "The Core Functionality")
as_add_module(core)

set(extra_libs "")

include_directories(${LOG_INCLUDE_DIR})

list(APPEND extra_libs ${OpenCV_LIBS} ${Boost_LIBRARIES} ${YAML_CPP_LIBRARIES})

if(WINRT AND WINDOWS_STORE AND CMAKE_SYSTEM_VERSION MATCHES "8.0")
  list(APPEND extra_libs ole32.lib)
endif()

#if(HAVE_TBB)
#  list(APPEND extra_libs tbb)
#endif()

if(DEFINED WINRT AND NOT DEFINED ENABLE_WINRT_MODE_NATIVE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /ZW")
endif()

#if(HAVE_CUDA)
#  if(NOT HAVE_as_cudev)
#    message(FATAL_ERROR "CUDA: AS requires enabled 'cudev' module from 'as_contrib' repository: https://github.com/as/as_contrib")
#  endif()
#  as_warnings_disable(CMAKE_CXX_FLAGS -Wundef -Wenum-compare -Wunused-function -Wshadow)
#endif()

if(CV_TRACE AND HAVE_ITT)
  add_definitions(-DAS_WITH_ITT=1)
endif()



file(GLOB lib_hdrs_vision
      "${CMAKE_CURRENT_LIST_DIR}/include/as/${name}/imgproc/*.hpp"
      "${CMAKE_CURRENT_LIST_DIR}/include/as/${name}/imgproc/*.h"
)
source_group("Include\\as\\${name}\\imgproc"  FILES ${lib_hdrs_vision})

as_source_group("Include" DIRBASE "${CMAKE_CURRENT_LIST_DIR}/include" FILES ${core_parallel_hdrs})

source_group("Src" FILES "${AS_MODULE_as_core_BINARY_DIR}/version_string.inc")

as_glob_module_sources(SOURCES "${AS_MODULE_as_core_BINARY_DIR}/version_string.inc"
                        HEADERS ${core_parallel_hdrs} ${module_opencl_hdrs} ${lib_cuda_hdrs} ${lib_cuda_hdrs_detail} ${lib_hdrs_vision} ${OpenCV_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIRS} ${YAML_CPP_INCLUDE_DIR})



as_module_include_directories(${the_module} ${ZLIB_INCLUDE_DIRS} ${OPENCL_INCLUDE_DIRS} ${Opencv_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIRS} ${YAML_CPP_INCLUDE_DIR})


file(GLOB_RECURSE ${core_ext_hdrs}
	"${CMAKE_CURRENT_LIST_DIR}/include/as/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/as/${name}/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/as/${name}/*.h" 
    "${CMAKE_CURRENT_LIST_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/*.h"    
)



as_source_group("Include" DIRBASE "${CMAKE_CURRENT_LIST_DIR}/include" 
		 FILES  ${core_ext_hdrs} ${lib_hdrs_libBinary})
		 

as_glob_module_sources(HEADERS ${core_ext_hdrs} ${lib_hdrs_vision} ${lib_hdrs_libBinary})

as_module_include_directories(${the_module} ${core_ext_hdrs} ${lib_hdrs_libBinary})



#在这添加了boost库
if(HAVE_POSIX_MEMALIGN)
  as_append_source_file_compile_definitions(${CMAKE_CURRENT_SOURCE_DIR}/src/alloc.cpp "HAVE_POSIX_MEMALIGN=1")
endif()
if(HAVE_MALLOC_H)
  as_append_source_file_compile_definitions(${CMAKE_CURRENT_SOURCE_DIR}/src/alloc.cpp "HAVE_MALLOC_H=1")
endif()
if(HAVE_MEMALIGN)
  as_append_source_file_compile_definitions(${CMAKE_CURRENT_SOURCE_DIR}/src/alloc.cpp "HAVE_MEMALIGN=1")
endif()
if(HAVE_WIN32_ALIGNED_MALLOC)
  as_append_source_file_compile_definitions(${CMAKE_CURRENT_SOURCE_DIR}/src/alloc.cpp "HAVE_WIN32_ALIGNED_MALLOC=1")
endif()
if(HAVE_VA_INTEL_OLD_HEADER)
  as_append_source_file_compile_definitions("${CMAKE_CURRENT_LIST_DIR}/src/va_intel.cpp" "HAVE_VA_INTEL_OLD_HEADER")
endif()
if(AS_LIBVA_LINK)
  as_append_source_file_compile_definitions("${CMAKE_CURRENT_LIST_DIR}/src/va_intel.cpp" "AS_LIBVA_LINK=1")
endif()

option(AS_ENABLE_ALLOCATOR_STATS "Enable Allocator metrics" ON)

if(NOT AS_ENABLE_ALLOCATOR_STATS)
  add_definitions(-DAS_DISABLE_ALLOCATOR_STATS=1)
elseif(HAVE_CXX11 OR DEFINED AS_ALLOCATOR_STATS_COUNTER_TYPE)
  if(NOT DEFINED AS_ALLOCATOR_STATS_COUNTER_TYPE)
    if(HAVE_ATOMIC_LONG_LONG AND AS_ENABLE_ATOMIC_LONG_LONG)
      if(MINGW)
        # command-line generation issue due to space in value, int/int64_t should be used instead
        # https://github.com/as/as/issues/16990
        message(STATUS "Consider adding AS_ALLOCATOR_STATS_COUNTER_TYPE=int/int64_t according to your build configuration")
      else()
        set(AS_ALLOCATOR_STATS_COUNTER_TYPE "long long")
      endif()
    else()
      set(AS_ALLOCATOR_STATS_COUNTER_TYPE "int")
    endif()
  endif()
  if(DEFINED AS_ALLOCATOR_STATS_COUNTER_TYPE)
    message(STATUS "Allocator metrics storage type: '${AS_ALLOCATOR_STATS_COUNTER_TYPE}'")
    add_definitions("-DAS_ALLOCATOR_STATS_COUNTER_TYPE=${AS_ALLOCATOR_STATS_COUNTER_TYPE}")
  endif()
endif()


if(PARALLEL_ENABLE_PLUGINS)
  as_append_source_file_compile_definitions(${CMAKE_CURRENT_SOURCE_DIR}/src/parallel/parallel.cpp "PARALLEL_ENABLE_PLUGINS=1")
  if(AS_DEBUG_POSTFIX)
    as_append_source_file_compile_definitions("${CMAKE_CURRENT_LIST_DIR}/src/parallel/parallel.cpp" "DEBUG_POSTFIX=${AS_DEBUG_POSTFIX}")
  endif()
endif()

as_create_module(${extra_libs})

#在这链接了boost库
as_target_link_libraries(${the_module} PRIVATE
    "${ZLIB_LIBRARIES}" "${OPENCL_LIBRARIES}" "${VA_LIBRARIES}"
    "${LAPACK_LIBRARIES}" "${CPUFEATURES_LIBRARIES}" "${HALIDE_LIBRARIES}"
    "${ITT_LIBRARIES}"
    "${AS_HAL_LINKER_LIBS}"
    "${Boost_LIBRARIES}"
    yaml-cpp	
)

if(AS_CORE_EXCLUDE_C_API)
  as_target_compile_definitions(${the_module} PRIVATE "AS_EXCLUDE_C_API=1")
endif()

if(HAVE_HPX)
  as_target_link_libraries(${the_module} LINK_PRIVATE "${HPX_LIBRARIES}")
endif()

as_add_accuracy_tests()
as_add_perf_tests()

as_install_3rdparty_licenses(SoftFloat "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/SoftFloat/COPYING.txt")


# generate data (samples data) config file
set(AS_DATA_CONFIG_FILE "${CMAKE_BINARY_DIR}/as_data_config.hpp")
set(AS_DATA_CONFIG_STR "")

if(CMAKE_INSTALL_PREFIX)
  set(AS_DATA_CONFIG_STR "${AS_DATA_CONFIG_STR}
#define AS_INSTALL_PREFIX \"${CMAKE_INSTALL_PREFIX}\"
")
endif()
if(AS_OTHER_INSTALL_PATH)
  set(AS_DATA_CONFIG_STR "${AS_DATA_CONFIG_STR}
#define AS_DATA_INSTALL_PATH \"${AS_OTHER_INSTALL_PATH}\"
")
endif()

set(AS_DATA_CONFIG_STR "${AS_DATA_CONFIG_STR}
#define AS_BUILD_DIR \"${CMAKE_BINARY_DIR}\"
")

file(RELATIVE_PATH SOURCE_DIR_RELATIVE ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR})
set(AS_DATA_CONFIG_STR "${AS_DATA_CONFIG_STR}
#define AS_DATA_BUILD_DIR_SEARCH_PATHS \\
    \"${SOURCE_DIR_RELATIVE}/\"
")

if(WIN32)
  file(RELATIVE_PATH INSTALL_DATA_DIR_RELATIVE "${CMAKE_INSTALL_PREFIX}/${AS_BIN_INSTALL_PATH}" "${CMAKE_INSTALL_PREFIX}/${AS_OTHER_INSTALL_PATH}")
else()
  file(RELATIVE_PATH INSTALL_DATA_DIR_RELATIVE "${CMAKE_INSTALL_PREFIX}/${AS_LIB_INSTALL_PATH}" "${CMAKE_INSTALL_PREFIX}/${AS_OTHER_INSTALL_PATH}")
endif()
list(APPEND AS_INSTALL_DATA_DIR_RELATIVE "${INSTALL_DATA_DIR_RELATIVE}")
string(REPLACE ";" "\",\\\n    \"" AS_INSTALL_DATA_DIR_RELATIVE_STR "\"${AS_INSTALL_DATA_DIR_RELATIVE}\"")
set(AS_DATA_CONFIG_STR "${AS_DATA_CONFIG_STR}
#define AS_INSTALL_DATA_DIR_RELATIVE ${AS_INSTALL_DATA_DIR_RELATIVE_STR}
")

if(EXISTS "${AS_DATA_CONFIG_FILE}")
  file(READ "${AS_DATA_CONFIG_FILE}" __content)
endif()
if(NOT AS_DATA_CONFIG_STR STREQUAL "${__content}")
  file(WRITE "${AS_DATA_CONFIG_FILE}" "${AS_DATA_CONFIG_STR}")
endif()
