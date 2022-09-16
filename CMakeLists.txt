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
)

add_library(sc-component-manager-lib SHARED ${SOURCES})

include_directories(${GLIB2_INCLUDE_DIRS} ${SC_COMPONENT_MANAGER_ROOT}/../sc-config-utils)
target_link_libraries(sc-component-manager-lib sc-memory sc-agents-common sc-builder-lib)
add_dependencies(sc-component-manager-lib sc-code-generator)

target_link_libraries(sc-component-manager sc-component-manager-lib sc-config-utils)

sc_codegen_ex(sc-component-manager
    "${CMAKE_CURRENT_SOURCE_DIR}/src/manager/commands/keynodes/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/manager/commands/keynodes/generated"
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sc-component-manager-lib)
endif()

if(${SC_BUILD_TESTS})
    include(tests/tests.cmake)
endif()
