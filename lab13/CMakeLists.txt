cmake_minimum_required(VERSION 3.10)
project(warehouse_project)

option(BUILD_SHARED_LIBS "Build shared library (.so/.dll)" ON)

include_directories(include)

set(LIB_SOURCES src/warehouse.c)

if(BUILD_SHARED_LIBS)
    add_library(warehouse SHARED ${LIB_SOURCES})
    message(STATUS "Building SHARED library")
else()
    add_library(warehouse STATIC ${LIB_SOURCES})
    message(STATUS "Building STATIC library")
endif()

add_executable(main src/main.c)

target_link_libraries(main warehouse)