make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME install-module-tests
    DEPENDS install-module
    INCLUDES ${INSTALL_MODULE_SRC}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(install-module-tests)
endif()
