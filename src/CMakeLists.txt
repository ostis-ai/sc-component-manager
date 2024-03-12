set(COMMON_MODULE_SRC ${CMAKE_CURRENT_LIST_DIR}/manager/agents/common-module)
set(INTERFACE_SRC ${CMAKE_CURRENT_LIST_DIR}/manager/interface_manager)

add_subdirectory(manager/agents)
add_subdirectory(manager/interface_manager)

set(SC_COMPONENT_MANAGER_SRC "${CMAKE_SOURCE_DIR}/src")

include_directories(${SC_COMPONENT_MANAGER_SRC} ${SC_MEMORY_SRC} ${GLIB2_INCLUDE_DIRS} ${SC_TOOLS_SRC} ${COMMON_MODULE_SRC} ${INTERFACE_SRC})

add_executable(sc-component-manager "${SC_COMPONENT_MANAGER_SRC}/main.cpp")
target_link_libraries(sc-component-manager sc-memory ${GLIB2_LIBRARIES} sc-config-utils interface)

set_target_properties(sc-component-manager PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SC_BIN_PATH})

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sc-component-manager)
endif()
