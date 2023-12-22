


cmake_minimum_required(VERSION 3.15)
project(ContainerTests LANGUAGES CXX)

# Include GoogleTest
include(FetchContent)
FetchContent_Declare(
    GoogleTest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(GoogleTest)

find_package(Container CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})

include(GoogleTest)

file(GLOB TestPaths ${CMAKE_CURRENT_SOURCE_DIR}/*.test.cpp)
foreach (TestPath ${TestPaths})
    get_filename_component(TestFileName ${TestPath} NAME_WLE)
    add_executable(${TestFileName} ${TestPath})
    target_include_directories(${TestFileName}
        PRIVATE ${Container_INCLUDE_DIR}
    )
    target_link_libraries(${TestFileName}
        PRIVATE CompilerFlags
        PRIVATE Container
        PRIVATE GTest::gtest_main
    )
endforeach (TestPath ${TestPaths})