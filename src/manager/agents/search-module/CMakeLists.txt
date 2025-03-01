file(GLOB SOURCES CONFIGURE_DEPENDS
    "module/*.hpp" "module/*.cpp"
    "module/*/*.hpp" "module/*/*.cpp"
    "utils/*.hpp" "utils/*.cpp"
)

add_library(search-module SHARED ${SOURCES})
target_link_libraries(search-module LINK_PUBLIC common-lib)
target_include_directories(search-module PRIVATE ${CMAKE_CURRENT_LIST_DIR})
set_target_properties(search-module PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(search-module)
endif()

if(${SC_BUILD_TESTS})
    set(SEARCH_MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/module)
    add_subdirectory(test)
endif()
