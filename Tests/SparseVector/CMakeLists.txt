


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
    )
endforeach (TestPath ${TestPaths})