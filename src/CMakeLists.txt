set(COMMON_MODULE_SRC ${CMAKE_CURRENT_LIST_DIR}/manager/agents/common-module)
set(INTERFACE_SRC ${CMAKE_CURRENT_LIST_DIR}/manager/console-interface)
set(SC_COMPONENT_MANAGER_SRC "${CMAKE_SOURCE_DIR}/src")

add_subdirectory(manager/agents)
add_subdirectory(manager/console-interface)

add_executable(sc-component-manager "${SC_COMPONENT_MANAGER_SRC}/main.cpp")
target_link_libraries(sc-component-manager
    LINK_PRIVATE sc-memory
    LINK_PRIVATE sc-config-utils
    LINK_PRIVATE console-interface
)

set_target_properties(sc-component-manager PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SC_BIN_PATH})

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sc-component-manager)
endif()
