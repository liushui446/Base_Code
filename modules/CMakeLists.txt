add_definitions(-D__AS_BUILD=1)

if(NOT AS_MODULES_PATH)
    set(AS_MODULES_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

as_glob_modules(${AS_MODULES_PATH})

# build lists of modules to be documented
set(AS_MODULES_MAIN "")


foreach(mod ${AS_MODULES_BUILD})
  string(REGEX REPLACE "^as_" "" mod "${mod}")
  if("${AS_MODULE_as_${mod}_LOCATION}" STREQUAL "${AS_SOURCE_DIR}/modules/${mod}")
    list(APPEND AS_MODULES_MAIN ${mod})
  else()
    list(APPEND AS_MODULES_EXTRA ${mod})
  endif()
endforeach()
as_list_sort(AS_MODULES_MAIN)

set(FIXED_ORDER_MODULES core guid database)
list(REMOVE_ITEM AS_MODULES_MAIN ${FIXED_ORDER_MODULES})
set(AS_MODULES_MAIN ${FIXED_ORDER_MODULES} ${AS_MODULES_MAIN})

set(AS_MODULES_MAIN ${AS_MODULES_MAIN} CACHE INTERNAL "List of main modules" FORCE)

