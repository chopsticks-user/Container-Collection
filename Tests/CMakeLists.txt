


cmake_minimum_required(VERSION 3.15)
project(ContainerTests LANGUAGES CXX)

find_package(Container CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})

set(TestUtilsPath ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(DynamicBitset)
add_subdirectory(SparseVector)