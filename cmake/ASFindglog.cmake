set(glog_DIR "${CMAKE_SOURCE_DIR}/3rdparty/glog/lib/cmake/glog")

find_package(glog REQUIRED)

file(COPY ${CMAKE_SOURCE_DIR}/3rdparty/glog/bin/glog.dll DESTINATION ${PROJECT_BINARY_DIR}/bin/Debug)
file(COPY ${CMAKE_SOURCE_DIR}/3rdparty/glog/bin/glogd.dll DESTINATION ${PROJECT_BINARY_DIR}/bin/Debug)