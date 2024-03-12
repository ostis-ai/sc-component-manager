set(SC_COMPONENT_MANAGER_SRC "${CMAKE_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCES "${SC_COMPONENT_MANAGER_SRC}/manager/*.cpp")
file(GLOB_RECURSE HEADERS "${SC_COMPONENT_MANAGER_SRC}/manager/*.hpp")
list(REMOVE_ITEM SOURCES "${SC_COMPONENT_MANAGER_SRC}/main.cpp")

include_directories(${SC_COMPONENT_MANAGER_SRC} ${GLIB2_INCLUDE_DIRS} ${SC_MEMORY_SRC})

add_library(sc-component-manager-lib SHARED ${SOURCES} ${HEADERS})

target_link_libraries(sc-component-manager-lib sc-memory sc-core sc-config-utils sc-agents-common sc-builder-lib)

set(SC_COMPONENT_MANAGER_KEYNODES_SRC "${SC_COMPONENT_MANAGER_SRC}/manager/commands/keynodes")
sc_codegen_ex(sc-component-manager-lib "${SC_COMPONENT_MANAGER_KEYNODES_SRC}" "${SC_COMPONENT_MANAGER_KEYNODES_SRC}/generated")

add_executable(sc-component-manager "${SC_COMPONENT_MANAGER_SRC}/main.cpp")
target_link_libraries(sc-component-manager sc-component-manager-lib)

set_target_properties(sc-component-manager
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${SC_BIN_PATH}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sc-component-manager-lib)
    target_clangformat_setup(sc-component-manager)
endif()
