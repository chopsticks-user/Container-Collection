


cmake_minimum_required(VERSION 3.15)
project(Container VERSION 1.0.0 LANGUAGES CXX)

# include(GNUInstallDirs)
include(CMake/ContainerOptions.cmake)
include(CMake/CompilerFlags.cmake)

# Library
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
    ${PROJECT_SOURCE_DIR}/Source
    # $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}>
    # $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_subdirectory(Include)
add_subdirectory(Tests)
add_subdirectory(Examples)


