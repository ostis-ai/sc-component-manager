make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME console-interface-tests
    DEPENDS common-lib console-interface
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(console-interface-tests)
endif()
