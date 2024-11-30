cmake_minimum_required(VERSION 3.5.0)
project(FIR_Filter VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/FIRFilter.cpp
    src/main.cpp
)

add_executable(FIR_Filter ${SOURCES})

target_include_directories(FIR_Filter
    PUBLIC dependencies
    PUBLIC include)

include(CTest)
enable_testing()

