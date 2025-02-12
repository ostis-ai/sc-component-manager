file(GLOB SOURCES CONFIGURE_DEPENDS
    "*.hpp" "*.cpp"
    "*/*.hpp" "*/*.cpp"
    "*/*/*.hpp" "*/*/*.cpp"
)

add_library(init-lib SHARED ${SOURCES})
target_link_libraries(init-lib LINK_PUBLIC common-lib)
target_include_directories(init-lib PRIVATE ${CMAKE_CURRENT_LIST_DIR})
set_target_properties(init-lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(init-lib)
endif()
