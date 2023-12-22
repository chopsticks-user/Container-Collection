


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

# install(DIRECTORY ${PROJECT_SOURCE_DIR}/Source/ 
#     DESTINATION ${PROJECT_SOURCE_DIR}/Include/${PROJECT_NAME}
#     # PATTERN "CMakeLists.txt" EXCLUDE
# )
