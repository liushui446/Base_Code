set(LOG_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/Log/include/")
set(LOG_LIB_DIR "${CMAKE_SOURCE_DIR}/3rdparty/Log/lib/")

find_library(LOG_LIBRARY NAMES log4cpp PATHS "${CMAKE_SOURCE_DIR}/3rdparty/Log/lib/" )

find_library(LOG_LIBRARYD NAMES log4cppD PATHS "${CMAKE_SOURCE_DIR}/3rdparty/Log/lib/" )

set(LOG_LIB ${LOG_LIBRARY})
set(LOG_LIBD ${LOG_LIBRARYD})