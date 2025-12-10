cmake_minimum_required(VERSION 3.16)
project(DailyTradesReport)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE UTILS_SOURCE      src/utils/*.cpp)
file(GLOB_RECURSE STRUCTURES_SOURCE src/structures/*.cpp)

set(SOURCES
        src/PluginInterface.cpp
        ${UTILS_SOURCE}
        ${STRUCTURES_SOURCE}
)

add_library(DailyTradesReport SHARED ${SOURCES})

target_include_directories(DailyTradesReport PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/api
        ${CMAKE_SOURCE_DIR}/external
        ${CMAKE_SOURCE_DIR}/src
)
