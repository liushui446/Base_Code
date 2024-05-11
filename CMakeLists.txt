# -----------------------------------------------------------------------------
# Root CMake file for AS
#
#   From the off-tree build directory, invoke:
#     $ cmake <PAHT_TO_AS_ROOT>
#
# -----------------------------------------------------------------------------

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()

cmake_minimum_required(VERSION 3.16.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)
set(GUI_PACK FALSE CACHE BOOL "Pack GUI." FORCE)
set(OPTIMIZER_PACK TRUE CACHE BOOL "Pack Optimization Module.")
#
# Configure CMake policies
#
if(POLICY CMP0026)
  cmake_policy(SET CMP0026 NEW)
endif()

if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)  # CMake 3.0+ (2.8.12): MacOS "@rpath" in target's install name
endif()

if(POLICY CMP0046)
  cmake_policy(SET CMP0046 NEW)  # warn about non-existed dependencies
endif()

if(POLICY CMP0051)
  cmake_policy(SET CMP0051 NEW)
endif()

if(POLICY CMP0054)  # CMake 3.1: Only interpret if() arguments as variables or keywords when unquoted.
  cmake_policy(SET CMP0054 NEW)
endif()

if(POLICY CMP0056)
  cmake_policy(SET CMP0056 NEW)  # try_compile(): link flags
endif()

if(POLICY CMP0066)
  cmake_policy(SET CMP0066 NEW)  # CMake 3.7: try_compile(): use per-config flags, like CMAKE_CXX_FLAGS_RELEASE
endif()

if(POLICY CMP0067)
  cmake_policy(SET CMP0067 NEW)  # CMake 3.8: try_compile(): honor language standard variables (like C++11)
endif()

if(POLICY CMP0068)
  cmake_policy(SET CMP0068 NEW)  # CMake 3.9+: `RPATH` settings on macOS do not affect `install_name`.
endif()

if(POLICY CMP0075)
  cmake_policy(SET CMP0075 NEW)  # CMake 3.12+: Include file check macros honor `CMAKE_REQUIRED_LIBRARIES`
endif()

if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)  # CMake 3.13+: option() honors normal variables.
endif()

#
# Configure ASSPI CMake hooks
#
include(cmake/ASUtils.cmake)
as_cmake_reset_hooks()
as_check_environment_variables(AS_CMAKE_HOOKS_DIR)
if(DEFINED AS_CMAKE_HOOKS_DIR)
  foreach(__dir ${AS_CMAKE_HOOKS_DIR})
    get_filename_component(__dir "${__dir}" ABSOLUTE)
    as_cmake_hook_register_dir(${__dir})
  endforeach()
endif()

as_cmake_hook(CMAKE_INIT)

# must go before the project()/enable_language() commands
as_update(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
if(DEFINED CMAKE_BUILD_TYPE)
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${CMAKE_CONFIGURATION_TYPES}")
endif()

option(ENABLE_PIC "Generate position independent code (necessary for shared libraries)" TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE ${ENABLE_PIC})

as_cmake_hook(PRE_CMAKE_BOOTSTRAP)

# Bootstap CMake system: setup CMAKE_SYSTEM_NAME and other vars
if(AS_WORKAROUND_CMAKE_20989)
  set(CMAKE_SYSTEM_PROCESSOR_BACKUP ${CMAKE_SYSTEM_PROCESSOR})
endif()
enable_language(CXX C)
if(AS_WORKAROUND_CMAKE_20989)
  set(CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR_BACKUP})
endif()

as_cmake_hook(POST_CMAKE_BOOTSTRAP)

if(NOT AS_SKIP_CMAKE_SYSTEM_FILE)
  include("cmake/platforms/AS-${CMAKE_SYSTEM_NAME}.cmake" OPTIONAL RESULT_VARIABLE "AS_CMAKE_SYSTEM_FILE")
  if(NOT AS_CMAKE_SYSTEM_FILE)
    message(STATUS "AS: system-specific configuration file is not found: '${CMAKE_SYSTEM_NAME}'")
  endif()
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)  # https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT.html
  if(NOT CMAKE_TOOLCHAIN_FILE)
    if(WIN32)
      set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory" FORCE)
    else()
      set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation Directory" FORCE)
    endif()
  else()
    # any cross-compiling
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory" FORCE)
  endif()
endif()

enable_testing()

project(AS CXX C)

as_cmake_eval(DEBUG_PRE ONCE)

as_clear_vars(ASModules_TARGETS)

set(BUILD_LIST "" CACHE STRING "Build only listed modules (comma-separated, e.g. 'videoio,dnn,ts')")

# ----------------------------------------------------------------------------
# Break in case of popular CMake configuration mistakes
# ----------------------------------------------------------------------------
if(NOT CMAKE_SIZEOF_VOID_P GREATER 0)
  message(FATAL_ERROR "CMake fails to determine the bitness of the target platform.
  Please check your CMake and compiler installation. If you are cross-compiling then ensure that your CMake toolchain file correctly sets the compiler details.")
endif()

# ----------------------------------------------------------------------------
# Detect compiler and target platform architecture
# ----------------------------------------------------------------------------
include(cmake/ASDetectCXXCompiler.cmake)
as_cmake_hook(POST_DETECT_COMPILER)

# Add these standard paths to the search paths for FIND_LIBRARY
# to find libraries from these locations first
if(UNIX AND NOT ANDROID)
  if(X86_64 OR CMAKE_SIZEOF_VOID_P EQUAL 8)
    if(EXISTS /lib64)
      list(APPEND CMAKE_LIBRARY_PATH /lib64)
    else()
      list(APPEND CMAKE_LIBRARY_PATH /lib)
    endif()
    if(EXISTS /usr/lib64)
      list(APPEND CMAKE_LIBRARY_PATH /usr/lib64)
    else()
      list(APPEND CMAKE_LIBRARY_PATH /usr/lib)
    endif()
  elseif(X86 OR CMAKE_SIZEOF_VOID_P EQUAL 4)
    if(EXISTS /lib32)
      list(APPEND CMAKE_LIBRARY_PATH /lib32)
    else()
      list(APPEND CMAKE_LIBRARY_PATH /lib)
    endif()
    if(EXISTS /usr/lib32)
      list(APPEND CMAKE_LIBRARY_PATH /usr/lib32)
    else()
      list(APPEND CMAKE_LIBRARY_PATH /usr/lib)
    endif()
  endif()
endif()

# Add these standard paths to the search paths for FIND_PATH
# to find include files from these locations first
if(MINGW)
  if(EXISTS /mingw)
      list(APPEND CMAKE_INCLUDE_PATH /mingw)
  endif()
  if(EXISTS /mingw32)
      list(APPEND CMAKE_INCLUDE_PATH /mingw32)
  endif()
  if(EXISTS /mingw64)
      list(APPEND CMAKE_INCLUDE_PATH /mingw64)
  endif()
endif()


# AS build components
AS_OPTION(BUILD_SHARED_LIBS        "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)" NOT (ANDROID OR APPLE_FRAMEWORK))

# ASbuild options
# ===================================================
AS_OPTION(ENABLE_SOLUTION_FOLDERS    "Solution folder in Visual Studio or in other IDEs"        (MSVC_IDE OR CMAKE_GENERATOR MATCHES Xcode) )


# ----------------------------------------------------------------------------
#  Get actual AS version number from sources
# ----------------------------------------------------------------------------
include(cmake/ASVersion.cmake)

as_cmake_hook(POST_OPTIONS)

# ----------------------------------------------------------------------------
#  Build & install layouts
# ----------------------------------------------------------------------------

if(AS_TEST_DATA_PATH)
  get_filename_component(AS_TEST_DATA_PATH ${AS_TEST_DATA_PATH} ABSOLUTE)
endif()

# Save libs and executables in the same place
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" CACHE PATH "Output directory for applications")

if(ANDROID)
  set(LIBRARY_OUTPUT_PATH                "${AS_BINARY_DIR}/lib/${ANDROID_NDK_ABI_NAME}")
  as_update(3P_LIBRARY_OUTPUT_PATH      "${AS_BINARY_DIR}/3rdparty/lib/${ANDROID_NDK_ABI_NAME}")
else()
  set(LIBRARY_OUTPUT_PATH                "${AS_BINARY_DIR}/lib")
  as_update(3P_LIBRARY_OUTPUT_PATH      "${AS_BINARY_DIR}/3rdparty/lib")
endif()

if(ANDROID)
  if(ANDROID_ABI MATCHES "NEON")
    set(ENABLE_NEON ON)
  endif()
  if(ANDROID_ABI MATCHES "VFPV3")
    set(ENABLE_VFPV3 ON)
  endif()
endif()

if(WIN32)
  # Postfix of DLLs:
  as_update(AS_DLLVERSION "${AS_VERSION_MAJOR}${AS_VERSION_MINOR}${AS_VERSION_PATCH}")
  as_update(AS_DEBUG_POSTFIX d)
else()
  # Postfix of so's:
  as_update(AS_DLLVERSION "")
  as_update(AS_DEBUG_POSTFIX "")
endif()

if(DEFINED CMAKE_DEBUG_POSTFIX)
  set(AS_DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
endif()

if((INSTALL_CREATE_DISTRIB AND BUILD_SHARED_LIBS AND NOT DEFINED BUILD_as_world) OR APPLE_FRAMEWORK)
  set(BUILD_as_world ON CACHE INTERNAL "")
endif()

include(cmake/ASInstallLayout.cmake)

# ----------------------------------------------------------------------------
#  Path for build/platform -specific headers
# ----------------------------------------------------------------------------
as_update(AS_CONFIG_FILE_INCLUDE_DIR "${CMAKE_BINARY_DIR}/" CACHE PATH "Where to create the platform-dependant asconfig.h")
as_include_directories(${AS_CONFIG_FILE_INCLUDE_DIR})

# ----------------------------------------------------------------------------
#  Autodetect if we are in a GIT repository
# ----------------------------------------------------------------------------
find_host_package(Git QUIET)

if(NOT DEFINED AS_VCSVERSION AND GIT_FOUND)
  as_git_describe(AS_VCSVERSION "${AS_SOURCE_DIR}")
elseif(NOT DEFINED AS_VCSVERSION)
  # We don't have git:
  set(AS_VCSVERSION "unknown")
endif()

# ----------------------------------------------------------------------------
# AS compiler and linker options
# ----------------------------------------------------------------------------
# In case of Makefiles if the user does not setup CMAKE_BUILD_TYPE, assume it's Release:
if(CMAKE_GENERATOR MATCHES "Makefiles|Ninja" AND "${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE Release)
endif()

include(cmake/ASCompilerOptions.cmake)

as_cmake_hook(POST_CMAKE_BUILD_OPTIONS)


# ----------------------------------------------------------------------------
#       CHECK FOR SYSTEM LIBRARIES, OPTIONS, ETC..
# ----------------------------------------------------------------------------

if(UNIX)
  if(NOT APPLE_FRAMEWORK OR AS_ENABLE_PKG_CONFIG)
    if(CMAKE_CROSSCOMPILING AND NOT DEFINED ENV{PKG_CONFIG_LIBDIR} AND NOT DEFINED ENV{PKG_CONFIG_SYSROOT_DIR}
        AND NOT AS_ENABLE_PKG_CONFIG
    )
      if(NOT PkgConfig_FOUND)
        message(STATUS "AS disables pkg-config to avoid using of host libraries. Consider using PKG_CONFIG_LIBDIR to specify target SYSROOT")
      elseif(AS_SKIP_PKG_CONFIG_WARNING)
        message(WARNING "pkg-config is enabled in cross-compilation mode without defining of PKG_CONFIG_LIBDIR environment variable. This may lead to misconfigured host-based dependencies.")
      endif()
    elseif(AS_DISABLE_PKG_CONFIG)
      if(PkgConfig_FOUND)
        message(WARNING "AS_DISABLE_PKG_CONFIG flag has no effect")
      endif()
    else()
      find_package(PkgConfig QUIET)
    endif()
  endif()
  include(CheckFunctionExists)
  include(CheckIncludeFile)
  include(CheckSymbolExists)

  if(NOT APPLE)
    CHECK_INCLUDE_FILE(pthread.h HAVE_PTHREAD)
    if(ANDROID)
      set(AS_LINKER_LIBS ${AS_LINKER_LIBS} dl m log)
    elseif(CMAKE_SYSTEM_NAME MATCHES "FreeBSD|NetBSD|DragonFly|OpenBSD|Haiku")
      set(AS_LINKER_LIBS ${AS_LINKER_LIBS} m pthread)
    elseif(EMSCRIPTEN)
      # no need to link to system libs with emscripten
    else()
      set(AS_LINKER_LIBS ${AS_LINKER_LIBS} dl m pthread rt)
    endif()
  else()
    set(HAVE_PTHREAD 1)
  endif()

  if(AS_ENABLE_MEMALIGN)
    CHECK_SYMBOL_EXISTS(posix_memalign stdlib.h HAVE_POSIX_MEMALIGN)
    CHECK_INCLUDE_FILE(malloc.h HAVE_MALLOC_H)
    if(HAVE_MALLOC_H)
      CHECK_SYMBOL_EXISTS(memalign malloc.h HAVE_MEMALIGN)
    endif()
    # TODO:
    # - std::aligned_alloc() C++17 / C11
  endif()
elseif(WIN32)
  include(CheckIncludeFile)
  include(CheckSymbolExists)

  if(AS_ENABLE_MEMALIGN)
    CHECK_INCLUDE_FILE(malloc.h HAVE_MALLOC_H)
    if(HAVE_MALLOC_H)
      CHECK_SYMBOL_EXISTS(_aligned_malloc malloc.h HAVE_WIN32_ALIGNED_MALLOC)
    endif()
  endif()
endif()

include(cmake/ASPCHSupport.cmake)
include(cmake/ASModule.cmake)


# ----------------------------------------------------------------------------
#  Detect endianness of build platform
# ----------------------------------------------------------------------------

if(IOS)
  # test_big_endian needs try_compile, which doesn't work for iOS
  # http://public.kitware.com/Bug/view.php?id=12288
  set(WORDS_BIGENDIAN 0)
else()
  include(TestBigEndian)
  test_big_endian(WORDS_BIGENDIAN)
endif()

# ----------------------------------------------------------------------------
#  Detect 3rd-party libraries
# ----------------------------------------------------------------------------
include(cmake/ASFindOpenCV.cmake)
include(cmake/ASFindLibsGUI.cmake)
include(cmake/ASFindBoost.cmake)
include(cmake/ASFindSqlite.cmake)
include(cmake/ASFindLog.cmake)
include(cmake/ASFindgflags.cmake)
include(cmake/ASFindglog.cmake)
include(cmake/ASFindYAML.cmake)


# ----------------------------------------------------------------------------
# Code trace support
# ----------------------------------------------------------------------------
if(CV_TRACE)
  include(cmake/ASDetectTrace.cmake)
endif()

as_cmake_hook(POST_DETECT_DEPENDECIES)  # typo, deprecated (2019-06)
as_cmake_hook(POST_DETECT_DEPENDENCIES)

# ----------------------------------------------------------------------------
# Solution folders:
# ----------------------------------------------------------------------------
if(ENABLE_SOLUTION_FOLDERS)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")
endif()

# Extra AS targets: uninstall, package_source, perf, etc.
include(cmake/ASExtraTargets.cmake)


# ----------------------------------------------------------------------------
# Process subdirectories
# ----------------------------------------------------------------------------

# as.hpp and legacy headers
# add_subdirectory(include)

# Enable compiler options for AS modules/apps/samples only (ignore 3rdparty)
as_add_modules_compiler_options()

# AS modules
add_subdirectory(modules)

# AS applications
add_subdirectory(app)
# ----------------------------------------------------------------------------
# Finalization: generate configuration-based files
# ----------------------------------------------------------------------------

as_cmake_hook(PRE_FINALIZE)

# Generate platform-dependent and configuration-dependent headers
include(cmake/ASGenHeaders.cmake)

# Generate as.pc for pkg-config command
if(AS_GENERATE_PKGCONFIG)
  include(cmake/ASGenPkgconfig.cmake)
endif()

# Generate AS.mk for ndk-build (Android build tool)
#include(cmake/ASGenAndroidMK.cmake)

# Generate ASConfig.cmake and ASConfig-version.cmake for cmake projects
include(cmake/ASGenConfig.cmake)

# Generate Info.plist for the IOS framework
if(APPLE_FRAMEWORK)
  include(cmake/ASGenInfoPlist.cmake)
endif()

# Generate ABI descriptor
#include(cmake/ASGenABI.cmake)

# Generate environment setup file
if(INSTALL_TESTS AND AS_TEST_DATA_PATH)
  if(ANDROID)
    get_filename_component(TEST_PATH ${AS_TEST_INSTALL_PATH} DIRECTORY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/_run_all_tests_android.sh.in"
                   "${CMAKE_BINARY_DIR}/unix-install/as_run_all_tests.sh" @ONLY)
    install(PROGRAMS "${CMAKE_BINARY_DIR}/unix-install/as_run_all_tests.sh"
            DESTINATION ./ COMPONENT tests)
  elseif(WIN32)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/as_run_all_tests_windows.cmd.in"
                   "${CMAKE_BINARY_DIR}/win-install/as_run_all_tests.cmd" @ONLY)
    install(PROGRAMS "${CMAKE_BINARY_DIR}/win-install/as_run_all_tests.cmd"
            DESTINATION ${AS_TEST_INSTALL_PATH} COMPONENT tests)
  elseif(UNIX)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/as_run_all_tests_unix.sh.in"
                   "${CMAKE_BINARY_DIR}/unix-install/as_run_all_tests.sh" @ONLY)
    install(PROGRAMS "${CMAKE_BINARY_DIR}/unix-install/as_run_all_tests.sh"
            DESTINATION ${AS_TEST_INSTALL_PATH} COMPONENT tests)
  endif()
endif()

if(NOT AS_README_FILE)
  if(ANDROID)
    set(AS_README_FILE ${CMAKE_CURRENT_SOURCE_DIR}/platforms/android/README.android)
  endif()
endif()

if(NOT AS_LICENSE_FILE)
  set(AS_LICENSE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
endif()

# for UNIX it does not make sense as LICENSE and readme will be part of the package automatically
if(ANDROID OR NOT UNIX)
  install(FILES ${AS_LICENSE_FILE}
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
        DESTINATION ./ COMPONENT libs)
  if(AS_README_FILE)
    install(FILES ${AS_README_FILE}
            PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
            DESTINATION ./ COMPONENT libs)
  endif()
endif()

if(FALSE)
if(COMMAND as_pylint_finalize)
  as_pylint_add_directory(${CMAKE_CURRENT_LIST_DIR}/modules/python/test)
  as_pylint_add_directory(${CMAKE_CURRENT_LIST_DIR}/samples/python)
  as_pylint_add_directory(${CMAKE_CURRENT_LIST_DIR}/samples/dnn)
  as_pylint_add_directory_recurse(${CMAKE_CURRENT_LIST_DIR}/samples/python/tutorial_code)
  as_pylint_finalize()
endif()
if(TARGET check_pylint)
  message(STATUS "Registered 'check_pylint' target: using ${PYLINT_EXECUTABLE} (ver: ${PYLINT_VERSION}), checks: ${PYLINT_TOTAL_TARGETS}")
endif()
if(TARGET check_flake8)
  message(STATUS "Registered 'check_flake8' target: using ${FLAKE8_EXECUTABLE} (ver: ${FLAKE8_VERSION})")
endif()
endif()

if(AS_GENERATE_SETUPVARS)
  include(cmake/ASGenSetupVars.cmake)
endif()


# ----------------------------------------------------------------------------
# Summary:
# ----------------------------------------------------------------------------
status("")
status("General configuration for AS ${AS_VERSION} =====================================")
if(AS_VCSVERSION)
  status("  Version control:" ${AS_VCSVERSION})
endif()
if(AS_EXTRA_MODULES_PATH AND NOT BUILD_INFO_SKIP_EXTRA_MODULES)
  set(__dump_extra_header OFF)
  foreach(p ${AS_EXTRA_MODULES_PATH})
    if(EXISTS ${p})
      if(NOT __dump_extra_header)
        set(__dump_extra_header ON)
        status("")
        status("  Extra modules:")
      else()
        status("")
      endif()
      as_git_describe(EXTRA_MODULES_VCSVERSION "${p}")
      status("    Location (extra):" ${p})
      status("    Version control (extra):" ${EXTRA_MODULES_VCSVERSION})
    endif()
  endforeach()
  unset(__dump_extra_header)
endif()

# ========================== build platform ==========================
status("")
status("  Platform:")
if(NOT DEFINED AS_TIMESTAMP
    AND NOT CMAKE_VERSION VERSION_LESS 2.8.11
    AND NOT BUILD_INFO_SKIP_TIMESTAMP
)
  string(TIMESTAMP AS_TIMESTAMP "" UTC)
  set(AS_TIMESTAMP "${AS_TIMESTAMP}" CACHE STRING "Timestamp of AS build configuration" FORCE)
endif()
if(AS_TIMESTAMP)
  status("    Timestamp:"      ${AS_TIMESTAMP})
endif()
status("    Host:"             ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_VERSION} ${CMAKE_HOST_SYSTEM_PROCESSOR})
if(CMAKE_CROSSCOMPILING)
  status("    Target:"         ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION} ${CMAKE_SYSTEM_PROCESSOR})
endif()
status("    CMake:"            ${CMAKE_VERSION})
status("    CMake generator:"  ${CMAKE_GENERATOR})
status("    CMake build tool:" ${CMAKE_BUILD_TOOL})
if(MSVC)
  status("    MSVC:"           ${MSVC_VERSION})
endif()
if(CMAKE_GENERATOR MATCHES Xcode)
  status("    Xcode:"          ${XCODE_VERSION})
endif()
if(CMAKE_GENERATOR MATCHES "Xcode|Visual Studio|Multi-Config")
  status("    Configuration:"  ${CMAKE_CONFIGURATION_TYPES})
else()
  status("    Configuration:"  ${CMAKE_BUILD_TYPE})
endif()


# ========================= CPU code generation mode =========================
status("")
status("  CPU/HW features:")
status("    Baseline:"  "${CPU_BASELINE_FINAL}")
if(NOT CPU_BASELINE STREQUAL CPU_BASELINE_FINAL)
  status("      requested:"  "${CPU_BASELINE}")
endif()
if(CPU_BASELINE_REQUIRE)
  status("      required:"  "${CPU_BASELINE_REQUIRE}")
endif()
if(CPU_BASELINE_DISABLE)
  status("      disabled:"  "${CPU_BASELINE_DISABLE}")
endif()
if(CPU_DISPATCH_FINAL OR CPU_DISPATCH)
  status("    Dispatched code generation:"  "${CPU_DISPATCH_FINAL}")
  if(NOT CPU_DISPATCH STREQUAL CPU_DISPATCH_FINAL)
    status("      requested:"  "${CPU_DISPATCH}")
  endif()
  if(CPU_DISPATCH_REQUIRE)
    status("      required:"  "${CPU_DISPATCH_REQUIRE}")
  endif()
  foreach(OPT ${CPU_DISPATCH_FINAL})
    status("      ${OPT} (${CPU_${OPT}_USAGE_COUNT} files):"  "+ ${CPU_DISPATCH_${OPT}_INCLUDED}")
  endforeach()
endif()

# ========================== C/C++ options ==========================
if(CMAKE_CXX_COMPILER_VERSION)
  set(AS_COMPILER_STR "${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ARG1} (ver ${CMAKE_CXX_COMPILER_VERSION})")
else()
  set(AS_COMPILER_STR "${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ARG1}")
endif()
string(STRIP "${AS_COMPILER_STR}" AS_COMPILER_STR)

status("")
status("  C/C++:")
status("    Built as dynamic libs?:" BUILD_SHARED_LIBS THEN YES ELSE NO)
if(DEFINED CMAKE_CXX_STANDARD AND CMAKE_CXX_STANDARD)
  status("    C++ standard:"           "${CMAKE_CXX_STANDARD}")
endif()
status("    C++ Compiler:"           ${AS_COMPILER_STR})
status("    C++ flags (Release):"    ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE})
status("    C++ flags (Debug):"      ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG})
status("    C Compiler:"             ${CMAKE_C_COMPILER} ${CMAKE_C_COMPILER_ARG1})
status("    C flags (Release):"      ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE})
status("    C flags (Debug):"        ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG})
if(WIN32)
  status("    Linker flags (Release):" ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_RELEASE})
  status("    Linker flags (Debug):"   ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_DEBUG})
else()
  status("    Linker flags (Release):" ${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS_RELEASE})
  status("    Linker flags (Debug):"   ${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS_DEBUG})
endif()
status("    ccache:"                  AS_COMPILER_IS_CCACHE THEN YES ELSE NO)
status("    Precompiled headers:"     PCHSupport_FOUND AND ENABLE_PRECOMPILED_HEADERS THEN YES ELSE NO)

# ========================== Dependencies ============================
as_get_all_libs(deps_modules deps_extra deps_3rdparty)
status("    Extra dependencies:" ${deps_extra})
status("    3rdparty dependencies:" ${deps_3rdparty})

# ========================== AS modules ==========================
status("")
status("  AS modules:")
set(AS_MODULES_BUILD_ST "")
foreach(the_module ${AS_MODULES_BUILD})
  if(NOT AS_MODULE_${the_module}_CLASS STREQUAL "INTERNAL" OR the_module STREQUAL "as_ts")
    list(APPEND AS_MODULES_BUILD_ST "${the_module}")
  endif()
endforeach()
string(REPLACE "as_" "" AS_MODULES_BUILD_ST          "${AS_MODULES_BUILD_ST}")
string(REPLACE "as_" "" AS_MODULES_DISABLED_USER_ST  "${AS_MODULES_DISABLED_USER}")
string(REPLACE "as_" "" AS_MODULES_DISABLED_AUTO_ST  "${AS_MODULES_DISABLED_AUTO}")
string(REPLACE "as_" "" AS_MODULES_DISABLED_FORCE_ST "${AS_MODULES_DISABLED_FORCE}")
list(SORT AS_MODULES_BUILD_ST)
list(SORT AS_MODULES_DISABLED_USER_ST)
list(SORT AS_MODULES_DISABLED_AUTO_ST)
list(SORT AS_MODULES_DISABLED_FORCE_ST)
status("    To be built:"            AS_MODULES_BUILD          THEN ${AS_MODULES_BUILD_ST}          ELSE "-")
status("    Disabled:"               AS_MODULES_DISABLED_USER  THEN ${AS_MODULES_DISABLED_USER_ST}  ELSE "-")
status("    Disabled by dependency:" AS_MODULES_DISABLED_AUTO  THEN ${AS_MODULES_DISABLED_AUTO_ST}  ELSE "-")
status("    Unavailable:"            AS_MODULES_DISABLED_FORCE THEN ${AS_MODULES_DISABLED_FORCE_ST} ELSE "-")

as_build_features_string(apps_status
  IF BUILD_TESTS AND HAVE_as_ts THEN "tests"
  IF BUILD_PERF_TESTS AND HAVE_as_ts THEN "perf_tests"
  IF BUILD_EXAMPLES THEN "examples"
  IF BUILD_as_apps THEN "apps"
  IF BUILD_ANDROID_SERVICE THEN "android_service"
  IF (BUILD_ANDROID_EXAMPLES OR INSTALL_ANDROID_EXAMPLES) AND CAN_BUILD_ANDROID_PROJECTS THEN "android_examples"
  ELSE "-")
status("    Applications:" "${apps_status}")
as_build_features_string(docs_status
    IF TARGET doxygen_cpp THEN "doxygen"
    IF TARGET doxygen_python THEN "python"
    IF TARGET doxygen_javadoc THEN "javadoc"
    IF BUILD_as_js OR DEFINED AS_JS_LOCATION THEN "js"
    ELSE "NO"
)
status("    Documentation:" "${docs_status}")
status("    Non-free algorithms:" AS_ENABLE_NONFREE THEN "YES" ELSE "NO")


as_finalize_status()

if(ENABLE_CONFIG_VERIFICATION)
  as_verify_config()
endif()

if(HAVE_CUDA AND COMMAND CUDA_BUILD_CLEAN_TARGET)
  CUDA_BUILD_CLEAN_TARGET()
endif()

as_cmake_hook(POST_FINALIZE)