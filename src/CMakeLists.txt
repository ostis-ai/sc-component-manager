set(SC_COMPONENT_MANAGER_SRC "${CMAKE_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.hpp")
list(REMOVE_ITEM SOURCES "${SC_COMPONENT_MANAGER_SRC}/main.cpp")

add_library(sc-component-manager-lib SHARED ${SOURCES} ${HEADERS})
target_link_libraries(sc-component-manager-lib
    LINK_PUBLIC sc-memory
    LINK_PRIVATE sc-agents-common
    LINK_PUBLIC sc-builder-lib
    LINK_PUBLIC sc-config-utils
)
target_include_directories(sc-component-manager-lib
    PRIVATE ${SC_COMPONENT_MANAGER_SRC}
    PUBLIC ${SC_MEMORY_SRC}
    PUBLIC ${SC_KPM_SRC}
    PUBLIC ${SC_TOOLS_SRC}
)

set(SC_COMPONENT_MANAGER_KEYNODES_SRC "${SC_COMPONENT_MANAGER_SRC}/manager/commands/keynodes")
sc_codegen_ex(sc-component-manager-lib "${SC_COMPONENT_MANAGER_KEYNODES_SRC}" "${SC_COMPONENT_MANAGER_KEYNODES_SRC}/generated")

add_executable(sc-component-manager "${SC_COMPONENT_MANAGER_SRC}/main.cpp")
target_link_libraries(sc-component-manager PRIVATE sc-component-manager-lib)

set_target_properties(sc-component-manager PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SC_BIN_PATH})

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sc-component-manager-lib)
    target_clangformat_setup(sc-component-manager)
endif()
