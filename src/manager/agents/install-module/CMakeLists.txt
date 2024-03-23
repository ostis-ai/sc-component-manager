file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

add_library(install-lib SHARED ${SOURCES})
target_link_libraries(install-lib LINK_PUBLIC common-lib)
target_include_directories(install-lib PUBLIC ${CMAKE_CURRENT_LIST_DIR})

sc_codegen_ex(install-lib ${CMAKE_CURRENT_LIST_DIR}/module ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(install-lib)
endif()
