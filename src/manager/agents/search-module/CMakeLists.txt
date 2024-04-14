file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

add_library(search-lib SHARED ${SOURCES})
target_link_libraries(search-lib LINK_PUBLIC common-lib)
target_include_directories(search-lib PUBLIC ${CMAKE_CURRENT_LIST_DIR})

sc_codegen_ex(search-lib ${CMAKE_CURRENT_LIST_DIR}/module ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(search-lib)
endif()
