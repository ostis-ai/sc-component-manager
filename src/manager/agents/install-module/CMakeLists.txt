file(GLOB SOURCES CONFIGURE_DEPENDS
    "*.hpp" "*.cpp"
    "*/*.hpp" "*/*.cpp"
    "*/*/*.hpp" "*/*/*.cpp"
)

add_library(install-module SHARED ${SOURCES})
target_link_libraries(install-module LINK_PUBLIC common-lib)
target_include_directories(install-module PRIVATE ${CMAKE_CURRENT_LIST_DIR})
set_target_properties(install-module PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(install-module)
endif()
