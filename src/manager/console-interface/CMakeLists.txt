set(AGENTS_SRC "${SC_COMPONENT_MANAGER_SRC}/manager/agents")

file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

add_library(console-interface SHARED ${SOURCES})
target_link_libraries(console-interface
    LINK_PUBLIC sc-agents-common
    LINK_PUBLIC sc-builder-lib
    LINK_PUBLIC common-lib
)
target_include_directories(console-interface
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
    PUBLIC ${AGENTS_SRC}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(console-interface)
endif()
