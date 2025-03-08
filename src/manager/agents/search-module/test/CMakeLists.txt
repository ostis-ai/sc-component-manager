make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME search-module-tests
    DEPENDS search-module
    INCLUDES ${SEARCH_MODULE_SRC}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(search-module-tests)
endif()
