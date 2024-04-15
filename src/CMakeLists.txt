set(COMMON_MODULE_SRC ${CMAKE_CURRENT_LIST_DIR}/manager/agents/common-module)
set(INTERFACE_SRC ${CMAKE_CURRENT_LIST_DIR}/manager/console-interface)
set(SC_COMPONENT_MANAGER_SRC "${CMAKE_SOURCE_DIR}/src")

add_subdirectory(manager/agents)
add_subdirectory(manager/console-interface)

file(GLOB SOURCES CONFIGURE_DEPENDS "module/*.cpp" "module/*.hpp")

add_library(sc-component-manager SHARED ${SOURCES})
target_link_libraries(sc-component-manager
    LINK_PRIVATE sc-memory
    LINK_PRIVATE sc-config-utils
    LINK_PRIVATE console-interface
)
set(SC_COMPONENT_MANAGER_MODULE "${CMAKE_CURRENT_LIST_DIR}/module")
sc_codegen_ex(sc-component-manager "${SC_COMPONENT_MANAGER_MODULE}" "${SC_COMPONENT_MANAGER_MODULE}/generated")

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sc-component-manager)
endif()
