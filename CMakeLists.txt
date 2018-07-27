cmake_minimum_required(VERSION 3.10)
project(upnp)

include_directories (${CMAKE_SOURCE_DIR})

#boost
set(BOOST_ROOT "D:/boost_1_66_0")
set(BOOST_INCLUDEDIR "D:/boost_1_66_0/include/boost-1_66")
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
include_directories (${Boost_INCLUDE_DIR})
find_package (Boost 1.66.0 REQUIRED COMPONENTS date_time system regex)

#miniupnpc
set (UPNPC_BUILD_SHARED OFF CACHE BOOL "")
set (UPNPC_BUILD_TESTS OFF CACHE BOOL "")
set (UPNPC_BUILD_SAMPLE OFF CACHE BOOL "")
add_subdirectory(miniupnp/miniupnpc)

add_executable(upnp 
    src/main.cpp
    src/upnp.hpp
	src/server.hpp)

target_include_directories(upnp SYSTEM PRIVATE miniupnp/miniupnpc)

#set_target_properties (upnp PROPERTIES COMPILE_FLAGS -DMINIUPNP_STATICLIB)

target_link_libraries(upnp libminiupnpc-static ${Boost_REGEX_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_DATE_TIME_LIBRARY})