SET(AGENTS_SRC "${CMAKE_CURRENT_LIST_DIR}/../src/manager/agents")
SET(CONSOLE_SRC "${CMAKE_CURRENT_LIST_DIR}/../src/manager/console-interface")

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_LIST_DIR}/units/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/units/*.hpp")
add_executable(sc-component-manager-test ${SOURCES})

target_link_libraries(sc-component-manager-test sc-agents-common sc-memory sc-core common-lib search-lib install-lib init-lib gtest_main)

target_include_directories(
        sc-component-manager-test
        PRIVATE ${SC_MEMORY_SRC}/tests/sc-memory/_test ${GLIB2_LIBRARIES} ${AGENTS_SRC} ${CONSOLE_SRC})

set_target_properties(sc-component-manager-test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SC_BIN_PATH})

add_test(NAME sc-component-manager-test COMMAND sc-component-manager-test)
if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sc-component-manager-tests)
endif()

add_definitions(-DMODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")
