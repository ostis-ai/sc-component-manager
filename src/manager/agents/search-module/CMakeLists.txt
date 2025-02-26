file(GLOB SOURCES CONFIGURE_DEPENDS
    "*.hpp" "*.cpp"
    "*/*.hpp" "*/*.cpp"
    "*/*/*.hpp" "*/*/*.cpp"
)

add_library(search-module SHARED ${SOURCES})
target_link_libraries(search-module LINK_PUBLIC common-lib)
target_include_directories(search-module PRIVATE ${CMAKE_CURRENT_LIST_DIR})
set_target_properties(search-module PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(search-module)
endif()
