# ----------------------------------------------------------------------------
#  Detect 3rd-party GUI libraries
# ----------------------------------------------------------------------------


# --- QT4 ---
#aoi_clear_vars(HAVE_QT HAVE_QT6)


#find_package(Qt6 COMPONENTS  Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets UiTools Network REQUIRED)
 # set up a mapping so that the Release configuration for the Qt imported target is
 # used in the COVERAGE CMake configuration.

  set(HAVE_QT5 ON)
  set(HAVE_QT ON)
  set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
  #set_target_properties(Qt6::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")


