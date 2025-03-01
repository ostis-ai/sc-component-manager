set(AGENTS_SRC "${SC_COMPONENT_MANAGER_SRC}/manager/agents")

file(GLOB SOURCES CONFIGURE_DEPENDS
    "src/*.hpp" "src/*.cpp"
    "include/console-interface/*.hpp" "include/console-interface/*.cpp"
)

# Define console-interface as an object library instead of a shared library
# because it contains functions that are tightly coupled with other parts of the project only.
# This approach avoids the need for dynamic loading and path management.
add_library(console-interface OBJECT ${SOURCES})
target_link_libraries(console-interface
    LINK_PUBLIC common-lib
)
target_include_directories(console-interface
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC $<BUILD_INTERFACE:${AGENTS_SRC}>
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(console-interface)
endif()

if(${SC_BUILD_TESTS})
    add_subdirectory(test)
endif()
