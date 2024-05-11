# 
# (1) Install boost follow the "https://www.boost.org/doc/libs/1_76_0/more/getting_started/windows.html#simplified-build-from-source"
#   or "https://www.boost.org/doc/libs/1_76_0/more/getting_started/unix-variants.html"
# (2) add path ${Boost_Install_path}/lib/cmake/Boost-1.76.0/ to system variable ${CMAKE_PREFIX_PATH}

# Todo : list required components instead of find all components
find_package(Boost #REQUIRED 
    COMPONENTS ALL) 
