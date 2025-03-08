make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME init-module-tests
    DEPENDS init-module
    INCLUDES ${INIT_MODULE_SRC}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(init-module-tests)
endif()
