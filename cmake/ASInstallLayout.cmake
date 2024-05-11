# message(STATUS "Initial install layout:")
# as_cmake_dump_vars("AS_.*_INSTALL_PATH")

if(ANDROID)

  as_update(AS_BIN_INSTALL_PATH            "sdk/native/bin/${ANDROID_NDK_ABI_NAME}")
  as_update(AS_TEST_INSTALL_PATH           "${AS_BIN_INSTALL_PATH}")
  as_update(AS_SAMPLES_BIN_INSTALL_PATH    "sdk/native/samples/${ANDROID_NDK_ABI_NAME}")
  as_update(AS_LIB_INSTALL_PATH            "sdk/native/libs/${ANDROID_NDK_ABI_NAME}")
  as_update(AS_LIB_ARCHIVE_INSTALL_PATH    "sdk/native/staticlibs/${ANDROID_NDK_ABI_NAME}")
  as_update(AS_3P_LIB_INSTALL_PATH         "sdk/native/3rdparty/libs/${ANDROID_NDK_ABI_NAME}")
  as_update(AS_CONFIG_INSTALL_PATH         "sdk/native/jni")
  as_update(AS_INCLUDE_INSTALL_PATH        "sdk/native/jni/include")
  as_update(AS_OTHER_INSTALL_PATH          "sdk/etc")
  as_update(AS_SAMPLES_SRC_INSTALL_PATH    "samples/native")
  as_update(AS_LICENSES_INSTALL_PATH       "${AS_OTHER_INSTALL_PATH}/licenses")
  as_update(AS_TEST_DATA_INSTALL_PATH      "${AS_OTHER_INSTALL_PATH}/testdata")
  as_update(AS_DOC_INSTALL_PATH            "doc")
  as_update(AS_JAR_INSTALL_PATH            ".")
  as_update(AS_JNI_INSTALL_PATH            "${AS_LIB_INSTALL_PATH}")
  as_update(AS_JNI_BIN_INSTALL_PATH        "${AS_JNI_INSTALL_PATH}")

elseif(WIN32 AND CMAKE_HOST_SYSTEM_NAME MATCHES Windows)

  if(DEFINED AS_RUNTIME AND DEFINED AS_ARCH)
    as_update(AS_INSTALL_BINARIES_PREFIX "${AS_ARCH}/${AS_RUNTIME}/")
  else()
    message(STATUS "Can't detect runtime and/or arch")
    as_update(AS_INSTALL_BINARIES_PREFIX "")
  endif()
  if(AS_STATIC)
    as_update(AS_INSTALL_BINARIES_SUFFIX "staticlib")
  else()
    as_update(AS_INSTALL_BINARIES_SUFFIX "lib")
  endif()
  if(INSTALL_CREATE_DISTRIB)
    set(_jni_suffix "/${AS_ARCH}")
  else()
    set(_jni_suffix "")
  endif()

  as_update(AS_BIN_INSTALL_PATH           "${AS_INSTALL_BINARIES_PREFIX}bin")
  as_update(AS_TEST_INSTALL_PATH          "${AS_BIN_INSTALL_PATH}")
  as_update(AS_SAMPLES_BIN_INSTALL_PATH   "${AS_INSTALL_BINARIES_PREFIX}samples")
  as_update(AS_LIB_INSTALL_PATH           "${AS_INSTALL_BINARIES_PREFIX}${AS_INSTALL_BINARIES_SUFFIX}")
  as_update(AS_LIB_ARCHIVE_INSTALL_PATH   "${AS_LIB_INSTALL_PATH}")
  as_update(AS_3P_LIB_INSTALL_PATH        "${AS_INSTALL_BINARIES_PREFIX}staticlib")
  as_update(AS_CONFIG_INSTALL_PATH        ".")
  as_update(AS_INCLUDE_INSTALL_PATH       "include")
  as_update(AS_OTHER_INSTALL_PATH         "etc")
  as_update(AS_SAMPLES_SRC_INSTALL_PATH   "samples")
  as_update(AS_LICENSES_INSTALL_PATH      "${AS_OTHER_INSTALL_PATH}/licenses")
  as_update(AS_TEST_DATA_INSTALL_PATH     "testdata")
  as_update(AS_DOC_INSTALL_PATH           "doc")
  as_update(AS_JAR_INSTALL_PATH           "java")
  as_update(AS_JNI_INSTALL_PATH           "java${_jni_suffix}")
  as_update(AS_JNI_BIN_INSTALL_PATH       "${AS_JNI_INSTALL_PATH}")

else() # UNIX

  include(GNUInstallDirs)
  as_update(AS_BIN_INSTALL_PATH           "bin")
  as_update(AS_TEST_INSTALL_PATH          "${AS_BIN_INSTALL_PATH}")
  as_update(AS_SAMPLES_BIN_INSTALL_PATH   "${AS_BIN_INSTALL_PATH}")
  as_update(AS_LIB_INSTALL_PATH           "${CMAKE_INSTALL_LIBDIR}")
  as_update(AS_LIB_ARCHIVE_INSTALL_PATH   "${AS_LIB_INSTALL_PATH}")
  as_update(AS_3P_LIB_INSTALL_PATH        "${AS_LIB_INSTALL_PATH}/as/3rdparty")
  as_update(AS_CONFIG_INSTALL_PATH        "${AS_LIB_INSTALL_PATH}/cmake/as")
  as_update(AS_INCLUDE_INSTALL_PATH       "${CMAKE_INSTALL_INCLUDEDIR}/as")
  as_update(AS_OTHER_INSTALL_PATH         "${CMAKE_INSTALL_DATAROOTDIR}/as")
  as_update(AS_SAMPLES_SRC_INSTALL_PATH   "${AS_OTHER_INSTALL_PATH}/samples")
  as_update(AS_LICENSES_INSTALL_PATH      "${CMAKE_INSTALL_DATAROOTDIR}/licenses/as")
  as_update(AS_TEST_DATA_INSTALL_PATH     "${AS_OTHER_INSTALL_PATH}/testdata")
  as_update(AS_DOC_INSTALL_PATH           "${CMAKE_INSTALL_DATAROOTDIR}/doc/as")
  as_update(AS_JAR_INSTALL_PATH           "${CMAKE_INSTALL_DATAROOTDIR}/java/as")
  as_update(AS_JNI_INSTALL_PATH           "${AS_JAR_INSTALL_PATH}")
  as_update(AS_JNI_BIN_INSTALL_PATH       "${AS_JNI_INSTALL_PATH}")

endif()

as_update(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${AS_LIB_INSTALL_PATH}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if(INSTALL_TO_MANGLED_PATHS)
  foreach(v
      AS_INCLUDE_INSTALL_PATH
      # file names include version (.so/.dll): AS_LIB_INSTALL_PATH
      AS_CONFIG_INSTALL_PATH
      AS_3P_LIB_INSTALL_PATH
      AS_SAMPLES_SRC_INSTALL_PATH
      AS_DOC_INSTALL_PATH
      # JAR file name includes version: AS_JAR_INSTALL_PATH
      AS_TEST_DATA_INSTALL_PATH
      AS_OTHER_INSTALL_PATH
    )
    string(REGEX REPLACE "as[0-9]*" "as-${AS_VERSION}" ${v} "${${v}}")
  endforeach()
endif()

# message(STATUS "Final install layout:")
# as_cmake_dump_vars("AS_.*_INSTALL_PATH")
