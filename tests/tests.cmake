make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME sc-component-manager-tests
    DEPENDS sc-agents-common sc-memory sc-component-manager-lib
    INCLUDES ${SC_MEMORY_SRC}/tests/sc-memory/_test ${GLIB2_LIBRARIES}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sc-component-manager-tests)
endif()

add_definitions(-DMODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")
add_definitions(-DMODULE_KB_PATH="${CMAKE_CURRENT_LIST_DIR}/../kb/")
