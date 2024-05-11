set(the_description "imgproc module")
as_add_module(imgproc as_core)

set(extra_libs "")

find_package(OpenMP)
if(OpenMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

as_glob_module_sources(HEADERS ${OpenCV_INCLUDE_DIRS})

include_directories(${LOG_INCLUDE_DIR})

as_module_include_directories(${OpenCV_INCLUDE_DIRS})
list(APPEND extra_libs ${OpenCV_LIBS})

file(GLOB_RECURSE lib_hdrs_lib
    "${CMAKE_CURRENT_LIST_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/*.h"    
)

as_source_group("Include" DIRBASE "${CMAKE_CURRENT_LIST_DIR}/include" 
                FILES ${lib_hdrs_common} ${lib_hdrs_defect} ${lib_hdrs_lib} ${lib_hdrs_mark} ${lib_hdrs_libBinary})

as_glob_module_sources(HEADERS ${lib_hdrs_common} ${lib_hdrs_defect} ${lib_hdrs_lib} ${lib_hdrs_mark} ${lib_hdrs_libBinary})

as_module_include_directories(${the_module} ${lib_hdrs_common} ${lib_hdrs_defect}${lib_hdrs_lib} ${lib_hdrs_mark} ${lib_hdrs_libBinary})

as_create_module(${extra_libs})
