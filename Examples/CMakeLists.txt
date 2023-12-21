


cmake_minimum_required(VERSION 3.10 )
project(ContainerExamples LANGUAGES CXX)

find_package(Container CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})

file(GLOB ExamplePaths ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
foreach (ExamplePath ${ExamplePaths})
    get_filename_component(ExampleFileName ${ExamplePath} NAME_WLE)
    add_executable(${ExampleFileName} ${ExamplePath})
    target_include_directories(${ExampleFileName}
        PRIVATE ${Container_INCLUDE_DIR}
    )
    target_link_libraries(${ExampleFileName}
        PRIVATE CompilerFlags
        PRIVATE Container
    )
endforeach (ExamplePath ${ExamplePaths})