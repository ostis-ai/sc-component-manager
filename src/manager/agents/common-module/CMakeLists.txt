file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

add_library(common-lib SHARED ${SOURCES})
target_link_libraries(common-lib
    LINK_PUBLIC sc-memory
    LINK_PUBLIC sc-agents-common
    LINK_PUBLIC sc-builder-lib
    LINK_PUBLIC sc-config-utils
)
target_include_directories(common-lib
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
    PUBLIC ${SC_MEMORY_SRC}
    PUBLIC ${SC_TOOLS_SRC}
    PUBLIC ${SC_KPM_SRC}
)

sc_codegen_ex(common-lib ${CMAKE_CURRENT_LIST_DIR}/module ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(common-lib)
endif()
