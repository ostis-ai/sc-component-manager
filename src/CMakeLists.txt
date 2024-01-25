set(SC_COMPONENT_MANAGER_SRC "${CMAKE_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.hpp")
list(REMOVE_ITEM SOURCES
    "${SC_COMPONENT_MANAGER_SRC}/main.cpp"
)
include_directories(${SC_COMPONENT_MANAGER_SRC} ${GLIB2_INCLUDE_DIRS} ${SC_MEMORY_SRC})

add_library(sc-component-manager-lib SHARED ${SOURCES} ${HEADERS})

target_link_libraries(sc-component-manager-lib sc-memory sc-config-utils sc-agents-common sc-builder-lib)
sc_codegen_ex(sc-component-manager-lib ${SC_COMPONENT_MANAGER_SRC} "${SC_COMPONENT_MANAGER_SRC}/generated")
add_executable(
    sc-component-manager
    "${SC_COMPONENT_MANAGER_SRC}/main.cpp"
)

target_link_libraries(sc-component-manager sc-component-manager-lib)

set_target_properties(sc-component-manager
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${SC_BIN_PATH}
)
