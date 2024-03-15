file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

include_directories(${CMAKE_CURRENT_LIST_DIR} ${SC_MEMORY_SRC} ${SC_KPM_SRC} ${SC_TOOLS_SRC} ${COMMON_MODULE_SRC})

add_library(init-lib SHARED ${SOURCES})
target_link_libraries(init-lib sc-memory sc-agents-common common-lib)

sc_codegen_ex(init-lib ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_CLANG_FORMAT_CODE})
  target_clangformat_setup(init-lib)
endif()
