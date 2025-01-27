set(AGENTS_SRC "${SC_COMPONENT_MANAGER_SRC}/manager/agents")

file(GLOB SOURCES CONFIGURE_DEPENDS
    "src/*.hpp" "src/*.cpp"
    "include/console-interface/*.hpp" "include/console-interface/*.cpp"
)

add_library(console-interface SHARED ${SOURCES})
target_link_libraries(console-interface
    LINK_PUBLIC sc-machine::sc-agents-common
    LINK_PUBLIC sc-machine::sc-builder-lib
    LINK_PUBLIC common-lib
)
target_include_directories(console-interface
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}
    PRIVATE ${AGENTS_SRC}
)
target_include_directories(console-interface
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(console-interface)
endif()
