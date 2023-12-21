


cmake_minimum_required(VERSION 3.10)
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

# Install
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        PUBLIC_HEADER DESTINATION 
            ${PROJECT_SOURCE_DIR}/Include/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/CMake/${PROJECT_NAME}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION CMake/${PROJECT_NAME}
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/Source/ 
    DESTINATION ${PROJECT_SOURCE_DIR}/Include/${PROJECT_NAME}
    # PATTERN "CMakeLists.txt" EXCLUDE
)

#Tests
add_subdirectory(Tests)

#Examples
add_subdirectory(Examples)


