# --------------------------------------------------------------------------------------------
#  Installation for CMake Module:  ASConfig.cmake
#  Part 1/3: ${BIN_DIR}/ASConfig.cmake              -> For use *without* "make install"
#  Part 2/3: ${BIN_DIR}/unix-install/ASConfig.cmake -> For use with "make install"
#  Part 3/3: ${BIN_DIR}/win-install/ASConfig.cmake  -> For use within binary installers/packages
# -------------------------------------------------------------------------------------------

if(INSTALL_TO_MANGLED_PATHS)
  set(AS_USE_MANGLED_PATHS_CONFIGCMAKE TRUE)
else()
  set(AS_USE_MANGLED_PATHS_CONFIGCMAKE FALSE)
endif()

if(HAVE_CUDA)
  as_cmake_configure("${CMAKE_CURRENT_LIST_DIR}/templates/ASConfig-CUDA.cmake.in" CUDA_CONFIGCMAKE @ONLY)
endif()

if(ANDROID)
  if(NOT ANDROID_NATIVE_API_LEVEL)
    set(AS_ANDROID_NATIVE_API_LEVEL_CONFIGCMAKE 0)
  else()
    set(AS_ANDROID_NATIVE_API_LEVEL_CONFIGCMAKE "${ANDROID_NATIVE_API_LEVEL}")
  endif()
  as_cmake_configure("${CMAKE_CURRENT_LIST_DIR}/templates/ASConfig-ANDROID.cmake.in" ANDROID_CONFIGCMAKE @ONLY)
endif()

set(AS_MODULES_CONFIGCMAKE ${AS_MODULES_PUBLIC})

if(BUILD_FAT_JAVA_LIB AND HAVE_as_java)
  list(APPEND AS_MODULES_CONFIGCMAKE as_java)
endif()

if(BUILD_OBJC AND HAVE_as_objc)
  list(APPEND AS_MODULES_CONFIGCMAKE as_objc)
endif()


# -------------------------------------------------------------------------------------------
#  Part 1/3: ${BIN_DIR}/ASConfig.cmake              -> For use *without* "make install"
# -------------------------------------------------------------------------------------------
set(AS_INCLUDE_DIRS_CONFIGCMAKE "\"${AS_CONFIG_FILE_INCLUDE_DIR}\" \"${AS_SOURCE_DIR}/include\"")

foreach(m ${AS_MODULES_BUILD})
  if(EXISTS "${AS_MODULE_${m}_LOCATION}/include")
    set(AS_INCLUDE_DIRS_CONFIGCMAKE "${AS_INCLUDE_DIRS_CONFIGCMAKE} \"${AS_MODULE_${m}_LOCATION}/include\"")
  endif()
endforeach()

export(EXPORT ASModules FILE "${CMAKE_BINARY_DIR}/ASModules.cmake")

if(TARGET ippicv AND NOT BUILD_SHARED_LIBS)
  set(USE_IPPICV TRUE)
  file(RELATIVE_PATH IPPICV_INSTALL_PATH_RELATIVE_CONFIGCMAKE "${CMAKE_BINARY_DIR}" "${IPPICV_LOCATION_PATH}")
  as_cmake_configure("${CMAKE_CURRENT_LIST_DIR}/templates/ASConfig-IPPICV.cmake.in" IPPICV_CONFIGCMAKE @ONLY)
else()
  set(USE_IPPICV FALSE)
endif()

if(TARGET ippiw AND NOT BUILD_SHARED_LIBS AND IPPIW_INSTALL_PATH)
  set(USE_IPPIW TRUE)
  file(RELATIVE_PATH IPPIW_INSTALL_PATH_RELATIVE_CONFIGCMAKE "${CMAKE_BINARY_DIR}" "${IPPIW_LOCATION_PATH}")
  as_cmake_configure("${CMAKE_CURRENT_LIST_DIR}/templates/ASConfig-IPPIW.cmake.in" IPPIW_CONFIGCMAKE @ONLY)
else()
  set(USE_IPPIW FALSE)
endif()

as_cmake_hook(PRE_CMAKE_CONFIG_BUILD)
configure_file("${AS_SOURCE_DIR}/cmake/templates/ASConfig.cmake.in" "${CMAKE_BINARY_DIR}/ASConfig.cmake" @ONLY)
#support for version checking when finding as. find_package(AS 2.3.1 EXACT) should now work.
configure_file("${AS_SOURCE_DIR}/cmake/templates/ASConfig-version.cmake.in" "${CMAKE_BINARY_DIR}/ASConfig-version.cmake" @ONLY)

# --------------------------------------------------------------------------------------------
#  Part 2/3: ${BIN_DIR}/unix-install/ASConfig.cmake -> For use *with* "make install"
# -------------------------------------------------------------------------------------------
file(RELATIVE_PATH AS_INSTALL_PATH_RELATIVE_CONFIGCMAKE "${CMAKE_INSTALL_PREFIX}/${AS_CONFIG_INSTALL_PATH}/" ${CMAKE_INSTALL_PREFIX})
if (IS_ABSOLUTE ${AS_INCLUDE_INSTALL_PATH})
  set(AS_INCLUDE_DIRS_CONFIGCMAKE "\"${AS_INCLUDE_INSTALL_PATH}\"")
else()
  set(AS_INCLUDE_DIRS_CONFIGCMAKE "\"\${AS_INSTALL_PATH}/${AS_INCLUDE_INSTALL_PATH}\"")
endif()

if(USE_IPPICV)
  file(RELATIVE_PATH IPPICV_INSTALL_PATH_RELATIVE_CONFIGCMAKE "${CMAKE_INSTALL_PREFIX}" "${IPPICV_INSTALL_PATH}")
  as_cmake_configure("${CMAKE_CURRENT_LIST_DIR}/templates/ASConfig-IPPICV.cmake.in" IPPICV_CONFIGCMAKE @ONLY)
endif()
if(USE_IPPIW)
  file(RELATIVE_PATH IPPIW_INSTALL_PATH_RELATIVE_CONFIGCMAKE "${CMAKE_INSTALL_PREFIX}" "${IPPIW_INSTALL_PATH}")
  as_cmake_configure("${CMAKE_CURRENT_LIST_DIR}/templates/ASConfig-IPPIW.cmake.in" IPPIW_CONFIGCMAKE @ONLY)
endif()

function(as_gen_config TMP_DIR NESTED_PATH ROOT_NAME)
  as_path_join(__install_nested "${AS_CONFIG_INSTALL_PATH}" "${NESTED_PATH}")
  as_path_join(__tmp_nested "${TMP_DIR}" "${NESTED_PATH}")

  file(RELATIVE_PATH AS_INSTALL_PATH_RELATIVE_CONFIGCMAKE "${CMAKE_INSTALL_PREFIX}/${__install_nested}" "${CMAKE_INSTALL_PREFIX}/")

  as_cmake_hook(PRE_CMAKE_CONFIG_INSTALL)
  configure_file("${AS_SOURCE_DIR}/cmake/templates/ASConfig-version.cmake.in" "${TMP_DIR}/ASConfig-version.cmake" @ONLY)

  configure_file("${AS_SOURCE_DIR}/cmake/templates/ASConfig.cmake.in" "${__tmp_nested}/ASConfig.cmake" @ONLY)
  install(EXPORT ASModules DESTINATION "${__install_nested}" FILE ASModules.cmake COMPONENT dev)
  install(FILES
      "${TMP_DIR}/ASConfig-version.cmake"
      "${__tmp_nested}/ASConfig.cmake"
      DESTINATION "${__install_nested}" COMPONENT dev)

  if(ROOT_NAME)
    # Root config file
    configure_file("${AS_SOURCE_DIR}/cmake/templates/${ROOT_NAME}" "${TMP_DIR}/ASConfig.cmake" @ONLY)
    install(FILES
        "${TMP_DIR}/ASConfig-version.cmake"
        "${TMP_DIR}/ASConfig.cmake"
        DESTINATION "${AS_CONFIG_INSTALL_PATH}" COMPONENT dev)
  endif()
endfunction()

if((CMAKE_HOST_SYSTEM_NAME MATCHES "Linux" OR UNIX) AND NOT ANDROID)
  as_gen_config("${CMAKE_BINARY_DIR}/unix-install" "" "")
endif()

if(ANDROID)
  as_gen_config("${CMAKE_BINARY_DIR}/unix-install" "abi-${ANDROID_NDK_ABI_NAME}" "ASConfig.root-ANDROID.cmake.in")
  install(FILES "${AS_SOURCE_DIR}/platforms/android/android.toolchain.cmake" DESTINATION "${AS_CONFIG_INSTALL_PATH}" COMPONENT dev)
endif()

# --------------------------------------------------------------------------------------------
#  Part 3/3: ${BIN_DIR}/win-install/ASConfig.cmake  -> For use within binary installers/packages
# --------------------------------------------------------------------------------------------
if(WIN32)
  if(CMAKE_HOST_SYSTEM_NAME MATCHES Windows AND NOT AS_SKIP_CMAKE_ROOT_CONFIG)
    as_gen_config("${CMAKE_BINARY_DIR}/win-install"
                   "${AS_INSTALL_BINARIES_PREFIX}${AS_INSTALL_BINARIES_SUFFIX}"
                   "ASConfig.root-WIN32.cmake.in")
  else()
    as_gen_config("${CMAKE_BINARY_DIR}/win-install" "" "")
  endif()
endif()
