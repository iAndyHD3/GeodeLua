cmake_minimum_required(VERSION 3.21)
include(CPM.cmake)

project(bro2json LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    src/cli.cpp
    src/gencpp.cpp
    src/genlua.cpp
)

target_compile_definitions(bro2json PRIVATE CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

CPMAddPackage("gh:geode-sdk/broma#0183881")
CPMAddPackage("gh:fmtlib/fmt#10.1.1")

target_link_libraries(${PROJECT_NAME} Broma fmt)