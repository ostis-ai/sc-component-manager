file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

include_directories(${CMAKE_CURRENT_LIST_DIR} ${SC_MEMORY_SRC} ${SC_KPM_SRC} ${GLIB2_INCLUDE_DIRS} ${COMMON_MODULE_SRC} ${SC_TOOLS_SRC})

add_library(interface SHARED ${SOURCES})
target_link_libraries(interface sc-memory sc-agents-common sc-builder-lib ${GLIB2_LIBRARIES} common init install-lib search)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(interface)
endif()
