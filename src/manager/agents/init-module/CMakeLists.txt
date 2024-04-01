file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

add_library(init-lib SHARED ${SOURCES})
target_link_libraries(init-lib sc-memory sc-agents-common common-lib ${GLIB2_LIBRARIES})
target_include_directories(init-lib PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${SC_MEMORY_SRC} ${SC_KPM_SRC} ${GLIB2_INCLUDE_DIRS} ${SC_TOOLS_SRC} ${AGENTS_SRC})

sc_codegen_ex(init-lib ${CMAKE_CURRENT_LIST_DIR}/module ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_CLANG_FORMAT_CODE})
  target_clangformat_setup(init-lib)
endif()
