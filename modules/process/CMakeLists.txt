add_definitions(-DNOMINMAX)

set(the_description "process module")
as_add_module(process as_core as_imgproc as_database)

set(extra_libs "")

as_glob_module_sources(HEADERS ${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${YAML_CPP_INCLUDE_DIR})

include_directories(${LOG_INCLUDE_DIR})

as_module_include_directories(${the_module} ${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${YAML_CPP_INCLUDE_DIR})

list(APPEND extra_libs ${OpenCV_LIBS} ${YAML_CPP_LIBRARIES}) 

file(GLOB_RECURSE lib_hdrs_lib
	"${CMAKE_CURRENT_LIST_DIR}/include/as/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/as/${name}/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/as/${name}/*.h" 
    "${CMAKE_CURRENT_LIST_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/*.h"    
)

#as_source_group("Src" FILES ${process_srcs} ${process_hdrs})
as_source_group("Include" DIRBASE "${CMAKE_CURRENT_LIST_DIR}/include" 
		 FILES ${lib_hdrs_vision} ${lib_hdrs_lib} ${lib_hdrs_libBinary})
		 
as_glob_module_sources(HEADERS ${lib_hdrs_vision} ${lib_hdrs_lib} ${lib_hdrs_libBinary})
as_module_include_directories(${the_module} ${lib_hdrs_vision} ${lib_hdrs_lib} ${lib_hdrs_libBinary})


as_create_module(${extra_libs})

as_target_link_libraries(${the_module} PRIVATE
    "${Boost_LIBRARIES}"
    yaml-cpp
)

