file(GLOB SOURCES CONFIGURE_DEPENDS
    "src/*.hpp" "src/*.cpp"
    "include/common/*.hpp" "include/common/*.cpp"
)

add_library(common-lib SHARED ${SOURCES})
target_link_libraries(common-lib
    LINK_PUBLIC sc-machine::sc-agents-common
    LINK_PUBLIC sc-machine::sc-builder-lib
    LINK_PUBLIC sc-machine::sc-config
)
target_include_directories(common-lib
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

file(GLOB MODULE_SOURCES CONFIGURE_DEPENDS
    "module/*.hpp" "module/*.cpp"
)

add_library(common-module SHARED ${MODULE_SOURCES})
target_link_libraries(common-module LINK_PRIVATE common-lib)
target_include_directories(common-module PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/module)
set_target_properties(common-module PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(common-lib)
endif()
