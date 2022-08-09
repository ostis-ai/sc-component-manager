set(SC_COMPONENT_MANAGER_ROOT ${CMAKE_CURRENT_LIST_DIR})
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "src/*.hpp")

list(REMOVE_ITEM SOURCES
    "${SC_COMPONENT_MANAGER_ROOT}/src/main.cpp"
)

include_directories(
    ${SC_COMPONENT_MANAGER_ROOT}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
    ${SC_TOOLS_SRC}
)

add_executable(
        sc-component-manager
        "${SC_COMPONENT_MANAGER_ROOT}/src/main.cpp"
        ${SOURCES}
)
include_directories(${GLIB2_INCLUDE_DIRS} ${SC_COMPONENT_MANAGER_ROOT}/../sc-config-utils)
target_link_libraries(sc-component-manager sc-memory sc-agents-common sc-config-utils)

