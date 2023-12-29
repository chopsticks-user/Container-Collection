


cmake_minimum_required(VERSION 3.15)
project(ContainerCollection_Examples LANGUAGES CXX)

file(GLOB ExamplePaths ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
foreach (ExamplePath ${ExamplePaths})
    get_filename_component(ExampleFileName ${ExamplePath} NAME_WLE)
    add_executable(${ExampleFileName} ${ExamplePath})
    target_link_libraries(${ExampleFileName}
        PRIVATE Nezumi
    )
endforeach (ExamplePath ${ExamplePaths})