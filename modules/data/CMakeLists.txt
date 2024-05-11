set(the_description "data module")
as_add_module(data as_core as_io)
set(extra_libs "")

file(GLOB_RECURSE lib_hdrs_lib
    "${CMAKE_CURRENT_LIST_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/*.h"    
)

as_source_group("Include" DIRBASE "${CMAKE_CURRENT_LIST_DIR}/include" 
                FILES ${lib_hdrs_lib} )

as_glob_module_sources(HEADERS ${lib_hdrs_lib})
as_module_include_directories(${the_module} ${lib_hdrs_lib})
list(APPEND extra_libs)
as_create_module(${extra_libs})