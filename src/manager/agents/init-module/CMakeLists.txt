file(GLOB SOURCES CONFIGURE_DEPENDS
    "agent/*.hpp" "agent/*.cpp"
    "module/*.hpp" "module/*.cpp"
    "utils/*.hpp" "utils/*.cpp"
)

add_library(init-module SHARED ${SOURCES})
target_link_libraries(init-module LINK_PUBLIC common-lib)
target_include_directories(init-module PRIVATE ${CMAKE_CURRENT_LIST_DIR})
set_target_properties(init-module PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(init-module)
endif()

if(${SC_BUILD_TESTS})
    set(INIT_MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(test)
endif()
