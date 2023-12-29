


cmake_minimum_required(VERSION 3.15)

project(Nezumi VERSION 1.0.0 LANGUAGES CXX)

include(CMake/Options.cmake)

# Library
include(CMake/CompilerFlags.cmake)

add_library(${Nezumi_TARGET_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${Nezumi_TARGET_NAME} 
    ALIAS ${Nezumi_TARGET_NAME}
)
target_include_directories(${Nezumi_TARGET_NAME} INTERFACE
    $<BUILD_INTERFACE:${Nezumi_INCLUDE_BUILD_DIR}>
    $<INSTALL_INTERFACE:${Nezumi_INCLUDE_INSTALL_DIR}>
)
target_link_libraries(${Nezumi_TARGET_NAME} 
    INTERFACE ${PROJECT_NAME}::CompilerFlags
)

# Install
include(CMakePackageConfigHelpers)

install(TARGETS ${Nezumi_TARGET_NAME}
        EXPORT ${Nezumi_TARGET_NAME}Targets
        PUBLIC_HEADER DESTINATION 
            ${Nezumi_INCLUDE_BUILD_DIR}/${Nezumi_TARGET_NAME}
)

write_basic_package_version_file("${Nezumi_TARGET_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)

configure_package_config_file(
    "${Nezumi_CMAKE_MODULE_PATH}/${Nezumi_TARGET_NAME}Config.cmake.in"
    "${Nezumi_BINARY_DIR}/${Nezumi_TARGET_NAME}Config.cmake"
    INSTALL_DESTINATION CMake/${Nezumi_TARGET_NAME}
)

# install(DIRECTORY ${PROJECT_SOURCE_DIR}/Source/ 
#     DESTINATION ${PROJECT_SOURCE_DIR}/Include/${PROJECT_NAME}
#     # PATTERN "CMakeLists.txt" EXCLUDE
# )


add_subdirectory(Examples)