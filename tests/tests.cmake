configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/units/test_defines.hpp.in"
    "${CMAKE_CURRENT_LIST_DIR}/units/test_defines.hpp"
)

make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME sc-component-manager-tests
    DEPENDS sc-memory sc-component-manager-lib
    INCLUDES ${SC_MEMORY_SRC} ${GLIB2_LIBRARIES}
)

if (${SC_CLANG_FORMAT_CODE})
	target_clangformat_setup(sc-component-manager-tests)
endif ()
