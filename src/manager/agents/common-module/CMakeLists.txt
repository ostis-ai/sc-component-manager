file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

include_directories(${CMAKE_CURRENT_LIST_DIR} ${SC_MEMORY_SRC} ${SC_KPM_SRC} ${GLIB2_INCLUDE_DIRS} ${SC_TOOLS_SRC})

add_library(common-lib SHARED ${SOURCES})
target_link_libraries(common-lib sc-agents-common sc-memory sc-builder-lib ${GLIB2_LIBRARIES})

sc_codegen_ex(common-lib ${CMAKE_CURRENT_LIST_DIR}/module ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_CLANG_FORMAT_CODE})
  target_clangformat_setup(common-lib)
endif()
