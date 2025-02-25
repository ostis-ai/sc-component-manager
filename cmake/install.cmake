install(TARGETS 
    common-lib console-interface
    EXPORT sc-component-managerExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# a target set for things we don't want to export to the consumers
install(TARGETS
    init-lib install-lib search-lib sc-component-manager
    EXPORT privateExport
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/extensions"
)

export(EXPORT sc-component-managerExport 
    FILE "${CMAKE_CURRENT_BINARY_DIR}/sc-component-managerTargets.cmake"
)

install(EXPORT sc-component-managerExport
    FILE sc-component-managerTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sc-component-manager
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/sc-component-manager-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${SC_COMPONENT_MANAGER_ROOT}/cmake/sc-component-manager-config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/sc-component-manager-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sc-component-manager
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/sc-component-manager-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/sc-component-manager-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sc-component-manager
)

set(CPACK_PACKAGE_NAME                  sc-component-manager)
set(CPACK_PACKAGE_VENDOR                "OSTIS AI")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY   "Component manager for ostis-systems")
set(CPACK_PACKAGE_INSTALL_DIRECTORY     ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR         ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR         ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH         ${PROJECT_VERSION_PATCH})
set(CPACK_VERBATIM_VARIABLES            TRUE)

if (WIN32)
    set(CPACK_GENERATOR     ZIP)
else()
    set(CPACK_GENERATOR     TGZ)
endif()
include(CPack)
