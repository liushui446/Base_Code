string(REPLACE "," ";" AS_INSTALL_APPS_LIST "${AS_INSTALL_APPS_LIST}")  # support comma-separated list (,) too

function(as_add_application the_target)
    cmake_parse_arguments(APP "" "" "MODULES;SRCS" ${ARGN})
    as_check_dependencies(${APP_MODULES})
    if(NOT AS_DEPENDENCIES_FOUND)
        as_debug_message("NOT AS_DEPENDENCIES_FOUND")
        return()
    endif()

    #project(${the_target})
    as_target_include_modules_recurse(${the_target} ${APP_MODULES})
    as_target_include_directories(${the_target} PRIVATE "${AS_SOURCE_DIR}/include/as")
    as_add_executable(${the_target} ${APP_SRCS})
    as_target_link_libraries(${the_target} ${APP_MODULES})
    set_target_properties(${the_target} PROPERTIES
                        DEBUG_POSTFIX "${AS_DEBUG_POSTFIX}"
                        ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                        RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
                        OUTPUT_NAME "${the_target}")

    if(ENABLE_SOLUTION_FOLDERS)
        set_target_properties(${the_target} PROPERTIES FOLDER "applications")
    endif()

    if(NOT INSTALL_CREATE_DISTRIB
        OR (AS_INSTALL_APPS_LIST STREQUAL "all" OR ";${AS_INSTALL_APPS_LIST};" MATCHES ";${the_target};")
    )
        install(TARGETS ${the_target} RUNTIME DESTINATION ${AS_BIN_INSTALL_PATH} COMPONENT dev)
    elseif(INSTALL_CREATE_DISTRIB)
        if(BUILD_SHARED_LIBS)
            install(TARGETS ${the_target} RUNTIME DESTINATION ${AS_BIN_INSTALL_PATH} CONFIGURATIONS Release COMPONENT dev)
        endif()
    endif()
endfunction()

link_libraries(${AS_LINKER_LIBS})

as_add_application(AS
    MODULES as_core as_gui as_database as_data as_imgproc as_io as_process
    SRCS main.cpp)