set(SQLITE_LIB_DIR "${CMAKE_SOURCE_DIR}/3rdparty/Sqlite/lib")
set(SQLITE_LIB1 "${SQLITE_LIB_DIR}/sqlite3.lib")

    set(SQLITE_LIB2 "${SQLITE_LIB_DIR}/fmt.lib")

    set(SQLITE_LIB2D "${SQLITE_LIB_DIR}/fmtd.lib")

file(COPY ${CMAKE_SOURCE_DIR}/3rdparty/Sqlite/lib/sqlite3.dll DESTINATION ${PROJECT_BINARY_DIR}/bin/Debug)
file(COPY ${CMAKE_SOURCE_DIR}/3rdparty/Sqlite/lib/sqlite3.dll DESTINATION ${PROJECT_BINARY_DIR}/bin/Release)


