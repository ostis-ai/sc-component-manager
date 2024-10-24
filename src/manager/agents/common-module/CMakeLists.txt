file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

add_library(common-lib SHARED ${SOURCES})
target_link_libraries(common-lib
    LINK_PUBLIC sc-core
    LINK_PUBLIC sc-memory
    LINK_PUBLIC sc-agents-common
    LINK_PUBLIC sc-builder-lib
    LINK_PUBLIC sc-config
)
target_include_directories(common-lib
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
    PUBLIC ${SC_MEMORY_SRC}
    PUBLIC ${SC_AGENTS_COMMON_PATH}
    PUBLIC ${SC_CORE_PATH}
    PUBLIC ${SC_BUILDER_PATH}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(common-lib)
endif()
