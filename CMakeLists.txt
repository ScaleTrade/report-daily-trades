cmake_minimum_required(VERSION 3.16)
project(DailyTradesReport)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(DailyTradesReport SHARED
        src/PluginInterface.cpp
        src/Utils.cpp
)

target_include_directories(DailyTradesReport PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/api
        ${CMAKE_SOURCE_DIR}/external
)
