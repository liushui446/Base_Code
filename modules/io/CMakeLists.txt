set(the_description "io module")
as_define_module(io as_core)

file(GLOB_RECURSE lib_hdrs_lib
	"${CMAKE_CURRENT_LIST_DIR}/include/as/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/as/${name}/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/as/${name}/*.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/*.h"    
)

#as_glob_module_sources(HEADERS ${LOG_INCLUDE_DIR})
include_directories(${LOG_INCLUDE_DIR})
#as_module_include_directories(${LOG_INCLUDE_DIR})



target_link_libraries( ${the_module}
    PUBLIC
        debug ${LOG_LIBD}
		optimized ${LOG_LIB}
)