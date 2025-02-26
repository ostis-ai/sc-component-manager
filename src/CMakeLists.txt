set(SC_COMPONENT_MANAGER_SRC "${CMAKE_SOURCE_DIR}/src")

add_subdirectory(manager/agents)
add_subdirectory(manager/console-interface)

file(GLOB SOURCES CONFIGURE_DEPENDS "module/*.cpp" "module/*.hpp")

add_library(sc-component-manager SHARED ${SOURCES})
target_link_libraries(sc-component-manager
    LINK_PRIVATE common-lib
    LINK_PRIVATE console-interface
)
set_target_properties(sc-component-manager PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sc-component-manager)
endif()

if(${SC_BUILD_TESTS})
    add_subdirectory(tests)
endif()
