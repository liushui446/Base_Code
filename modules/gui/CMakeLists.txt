set(the_description "Graphical User Interface")
as_add_module(gui as_core as_database as_io as_process as_imgproc)

# ----------------------------------------------------------------------------
#  CMake file for highgui. See root CMakeLists.txt
#   Some parts taken from version of Hartmut Seichter, HIT Lab NZ.
#   Jose Luis Blanco, 2008
# ----------------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)
# # Qt
set(CMAKE_AUTOUIC ON)
if(DEFINED WINRT AND NOT DEFINED ENABLE_WINRT_MODE_NATIVE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /ZW")
endif()
add_definitions(-D_USE_MATH_DEFINES)

# set(extra_libs "")

# find_package(OpenMP)
# if(OpenMP_FOUND)
    # message("OPENMP FOUND")
    # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
# endif()

#if(APPLE)
#  as_include_directories(${ZLIB_INCLUDE_DIRS})
#  list(APPEND GUI_LIBRARIES ${ZLIB_LIBRARIES})
#endif()

#if(HAVE_WEBP)
#  add_definitions(-DHAVE_WEBP)
#endif()

file(GLOB_RECURSE gui_ext_hdrs
    "${CMAKE_CURRENT_LIST_DIR}/include/as/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/as/CGraphic/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/*.h")

# Removing WinRT API headers by default
#list(REMOVE_ITEM gui_ext_hdrs "${CMAKE_CURRENT_LIST_DIR}/include/as/${name}/gui_winrt.hpp")


if(HAVE_QT5)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
    
  file(GLOB_RECURSE _UI_FILES ${CMAKE_CURRENT_LIST_DIR}/*.ui)
  QT5_WRAP_UI(_UI_OUTFILES ${_UI_FILES}) 

  file(GLOB_RECURSE _QRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.qrc)
  QT5_ADD_RESOURCES(_RCC_OUTFILES ${_QRC_FILES})

  file(GLOB_RECURSE _MOC_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.h ${CMAKE_CURRENT_LIST_DIR}/include/*.hpp)
  QT5_WRAP_CPP(_MOC_OUTFILES ${_MOC_FILES})
  
  list(APPEND gui_srcs
       ${_UI_OUTFILES}
       ${_MOC_OUTFILES}
       ${_RCC_OUTFILES})

  foreach(dt5_dep Core Gui Widgets Test UiTools Concurrent)
    add_definitions(${Qt5${dt5_dep}_DEFINITIONS})
    include_directories(${Qt5${dt5_dep}_INCLUDE_DIRS} ${LOG_INCLUDE_DIR})#在此添加了头文件
    list(APPEND GUI_LIBRARIES ${Qt5${dt5_dep}_LIBRARIES})
  endforeach()

elseif(HAVE_QT5_)
  # "Automoc" doesn't work properly with as_world build, use QT5_WRAP_CPP() directly
  #set(CMAKE_AUTOMOC ON)

  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  QT5_ADD_RESOURCES(_RCC_OUTFILES ${CMAKE_CURRENT_LIST_DIR}/src/window_QT.qrc)
  QT5_WRAP_CPP(_MOC_OUTFILES ${CMAKE_CURRENT_LIST_DIR}/src/window_QT.h)
  list(APPEND gui_srcs
       ${CMAKE_CURRENT_LIST_DIR}/src/window_QT.cpp
       ${CMAKE_CURRENT_LIST_DIR}/src/window_QT.h
       ${_MOC_OUTFILES}
       ${_RCC_OUTFILES})

  foreach(dt5_dep Core Gui Widgets Test Concurrent)
    add_definitions(${Qt5${dt5_dep}_DEFINITIONS})
    include_directories(${Qt5${dt5_dep}_INCLUDE_DIRS})
    list(APPEND GUI_LIBRARIES ${Qt5${dt5_dep}_LIBRARIES})
  endforeach()

  if(HAVE_QT_OPENGL)
    add_definitions(${Qt5OpenGL_DEFINITIONS})
    include_directories(${Qt5OpenGL_INCLUDE_DIRS})
    list(APPEND GUI_LIBRARIES ${Qt5OpenGL_LIBRARIES})
  endif()

elseif(HAVE_QT)
  if (HAVE_QT_OPENGL)
    set(QT_USE_QTOPENGL TRUE)
  endif()
  include(${QT_USE_FILE})

  QT4_ADD_RESOURCES(_RCC_OUTFILES ${CMAKE_CURRENT_LIST_DIR}/src/window_QT.qrc)
  QT4_WRAP_CPP(_MOC_OUTFILES ${CMAKE_CURRENT_LIST_DIR}/src/window_QT.h)

  list(APPEND GUI_LIBRARIES ${QT_LIBRARIES})
  list(APPEND gui_srcs ${CMAKE_CURRENT_LIST_DIR}/src/window_QT.cpp ${_MOC_OUTFILES} ${_RCC_OUTFILES})
  as_check_flag_support(CXX -Wno-missing-declarations _have_flag "")
  if(${_have_flag})
    set_source_files_properties(${_RCC_OUTFILES} PROPERTIES COMPILE_FLAGS -Wno-missing-declarations)
  endif()
elseif(WINRT)
  if(NOT WINRT_8_0)
    # Dependencies used by the implementation referenced
    # below are not available on WinRT 8.0.
    # Enabling it for WiRT 8.1+ only.

    # WinRT 8.1+ detected. Adding WinRT API header.
    message(STATUS "  ${name}: WinRT detected. Adding WinRT API header")
    list(APPEND gui_ext_hdrs "${CMAKE_CURRENT_LIST_DIR}/include/as/${name}/gui_winrt.hpp")


    list(APPEND gui_srcs
      ${CMAKE_CURRENT_LIST_DIR}/src/window_winrt.cpp
      ${CMAKE_CURRENT_LIST_DIR}/src/window_winrt_bridge.cpp)
    list(APPEND gui_hdrs
      ${CMAKE_CURRENT_LIST_DIR}/src/window_winrt_bridge.hpp)
  endif()

  # libraries below are neither available nor required
  # on ARM devices and/or Windows Phone
  if(WINRT_PHONE OR (AS_ARCH STREQUAL "ARM"))
    list(REMOVE_ITEM GUI_LIBRARIES "comctl32" "gdi32" "ole32" "setupapi")
    if(WINRT_PHONE)
      message(STATUS "  ${name}: Windows Phone detected")
    elseif(AS_ARCH STREQUAL "ARM")
      message(STATUS "  ${name}: ARM detected")
      if(WINRT_STORE)
        list(REMOVE_ITEM GUI_LIBRARIES "ws2_32")
        message(STATUS "  ${name}:   Removing 'ws2_32.lib'")
      endif()
    endif()
    message(STATUS "  ${name}:   Removing 'comctl32.lib, gdi32.lib, ole32.lib, setupapi.lib'")
    message(STATUS "  ${name}:   Leaving '${GUI_LIBRARIES}'")
  endif()
elseif(HAVE_WIN32UI)
  list(APPEND gui_srcs ${CMAKE_CURRENT_LIST_DIR}/src/window_w32.cpp)
  if(AS_ARCH STREQUAL "ARM64")
    list(APPEND GUI_LIBRARIES "comdlg32" "advapi32")
  endif()
elseif(HAVE_GTK OR HAVE_GTK3)
  list(APPEND gui_srcs ${CMAKE_CURRENT_LIST_DIR}/src/window_gtk.cpp)
elseif(HAVE_COCOA)
  list(APPEND gui_srcs ${CMAKE_CURRENT_LIST_DIR}/src/window_cocoa.mm)
  list(APPEND GUI_LIBRARIES "-framework Cocoa")
endif()

list(APPEND GUI_LIBRARIES ${OpenCV_LIBS})

if(TRUE)
  # these variables are set by 'as_append_build_options(GUI ...)'
  foreach(P ${GUI_INCLUDE_DIRS})
    as_include_directories(${P})
  endforeach()

  foreach(P ${GUI_LIBRARY_DIRS})
    link_directories(${P})
  endforeach()
endif()

set(_INC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/module/file")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/module/edit")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/module/manufacture")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/module/machineControl")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/module/setting")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/module/others")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/res/button")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/res/dialog")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/res/general")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/../database/include")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/../process/include")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/../core/include")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/../data/include")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/../hardware/include")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/../imgproc/include")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/../io/include")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/../camera/include")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/../optimization/include")
file(GLOB _LOCAL_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*)
foreach(_FILE ${_LOCAL_FILES})
    file(GLOB _sub ${_FILE}/*)
    list(LENGTH _sub _after)
    if (NOT ${_after} EQUAL 0)
        list(APPEND _INC_DIR ${_FILE})
  endif()
endforeach()

include_directories("${_INC_DIR}" ${LOG_INCLUDE_DIR} ${GLOG_INCLUDE_DIR})

as_source_group("Src" FILES ${gui_srcs} ${gui_hdrs})
as_source_group("Include" DIRBASE "${CMAKE_CURRENT_LIST_DIR}/include" 
		 FILES ${gui_ext_hdrs})

as_glob_module_sources(HEADERS ${gui_ext_hdrs} ${OpenCV_INCLUDE_DIRS} SOURCES ${gui_srcs} ${gui_hdrs})
as_module_include_directories(${OpenCV_INCLUDE_DIRS})

if (OPTIMIZER_PACK)
    as_module_include_directories(${Opt_INCLUDE_DIR})
endif()

list(APPEND extra_libs ${OpenCV_LIBS})
#编辑gui目录下个文件夹可以使用相对路径
as_create_module(${GUI_LIBRARIES})
file(COPY ${CMAKE_CURRENT_LIST_DIR}/src/res  DESTINATION ${PROJECT_BINARY_DIR}/app)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/src/res  DESTINATION ${PROJECT_BINARY_DIR}/bin/Debug)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/src/res  DESTINATION ${PROJECT_BINARY_DIR}/bin/Release)

macro(as_gui_configure_target)
if(APPLE)
  add_apple_compiler_options(${the_module})
endif()

if(MSVC)
  set_target_properties(${the_module} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:atlthunk.lib /NODEFAULTLIB:atlsd.lib /NODEFAULTLIB:libcmt.lib /DEBUG")
endif()

as_warnings_disable(CMAKE_CXX_FLAGS -Wno-deprecated-declarations)
endmacro()

if(NOT BUILD_as_world)
  as_gui_configure_target()
endif()

as_add_accuracy_tests()
as_add_perf_tests()

find_package(
	Qt5 COMPONENTS		
        PrintSupport
        Charts
	REQUIRED
)

target_link_libraries( ${the_module}
    PRIVATE
        Qt5::PrintSupport
        Qt5::Charts
        Qt5::Network
	${Boost_LIBRARIES}
)

