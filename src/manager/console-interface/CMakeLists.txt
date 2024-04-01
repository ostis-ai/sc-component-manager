set(AGENTS_SRC "${SC_COMPONENT_MANAGER_SRC}/manager/agents")

file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

add_library(console-interface SHARED ${SOURCES})
target_link_libraries(console-interface sc-memory sc-agents-common sc-builder-lib ${GLIB2_LIBRARIES} common-lib init-lib install-lib search-lib)
target_include_directories(console-interface PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${SC_MEMORY_SRC} ${SC_KPM_SRC} ${GLIB2_INCLUDE_DIRS} ${SC_TOOLS_SRC} ${AGENTS_SRC})

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(console-interface)
endif()
