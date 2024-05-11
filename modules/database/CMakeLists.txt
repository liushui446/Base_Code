set(the_description "database module")
as_define_module(database as_core as_io)
as_add_module(database as_process as_imgproc)

source_group("Include\\as\\${name}\\imgproc"  FILES ${lib_hdrs_vision})

set(_INC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/../core/include")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/../process/include")
list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/../imgproc/include")
#list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/dao")
#list(APPEND _INC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/dao/impl")
include_directories("${_INC_DIR}")

target_link_libraries( ${the_module}
    PRIVATE
		${SQLITE_LIB1}
		${OpenCV_LIBS}
		debug ${SQLITE_LIB2D}
		optimized ${SQLITE_LIB2}
)
include_directories("${_INC_DIR}" ${LOG_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})