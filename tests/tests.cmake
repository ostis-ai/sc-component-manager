make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME sc-component-manager-tests
    DEPENDS sc-memory sc-core console-interface search-lib init-lib install-lib sc-builder-lib
    INCLUDES ${SC_MEMORY_SRC} ${SC_BUILDER_PATH}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sc-component-manager-tests)
endif()

add_definitions(-DMODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")
