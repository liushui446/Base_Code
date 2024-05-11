set(gflags_DIR "${CMAKE_SOURCE_DIR}/3rdparty/gflags/lib/cmake/gflags")

find_package(gflags REQUIRED)

file(COPY ${CMAKE_SOURCE_DIR}/3rdparty/gflags/bin/gflags.dll DESTINATION ${PROJECT_BINARY_DIR}/bin/Debug)
file(COPY ${CMAKE_SOURCE_DIR}/3rdparty/gflags/bin/gflags_debug.dll DESTINATION ${PROJECT_BINARY_DIR}/bin/Debug)
file(COPY ${CMAKE_SOURCE_DIR}/3rdparty/gflags/bin/gflags_nothreads.dll DESTINATION ${PROJECT_BINARY_DIR}/bin/Debug)
file(COPY ${CMAKE_SOURCE_DIR}/3rdparty/gflags/bin/gflags_nothreads_debug.dll DESTINATION ${PROJECT_BINARY_DIR}/bin/Debug)