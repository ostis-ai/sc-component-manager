make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME sc-component-manager-tests
    DEPENDS console-interface search-lib init-lib install-lib
    INCLUDES 
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sc-component-manager-tests)
endif()
