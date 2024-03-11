cmake_minimum_required(VERSION 2.8)
project(sc-component-manager)
set(CMAKE_CXX_STANDARD 17)

get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)

option(SC_BUILD_TESTS "Flag to build unit tests" OFF)
option(SC_CLANG_FORMAT_CODE "Flag to add clangformat and clangformat_check targets" OFF)

if(NOT SC_BIN_PATH)
    set(SC_BIN_PATH "${CMAKE_BINARY_DIR}/../bin")
endif()
set(SC_EXTENSIONS_DIRECTORY "${SC_BIN_PATH}/extensions")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})
link_directories(${SC_BIN_PATH} ${SC_EXTENSIONS_DIRECTORY})

set(SC_MACHINE_PATH "${PARENT_DIR}/sc-machine/")
include("${SC_MACHINE_PATH}/cmake/codegen.cmake")
set(SC_CODEGEN_TOOL "${SC_BIN_PATH}/sc-code-generator")

if(${SC_BUILD_TESTS})
    include(${CMAKE_MODULE_PATH}/tests.cmake)
endif()

if(${SC_CLANG_FORMAT_CODE})
    include(${SC_MACHINE_PATH}/cmake/ClangFormat.cmake)
endif()

set(SC_MEMORY_SRC "${SC_MACHINE_PATH}/sc-memory/")
set(SC_KPM_SRC "${SC_MACHINE_PATH}/sc-kpm/")
set(SC_TOOLS_SRC "${SC_MACHINE_PATH}/sc-tools/")

include_directories(${SC_MEMORY_SRC} ${SC_KPM_SRC} ${SC_TOOLS_SRC})

if(${WIN32})
    message(SEND_ERROR "sc-component-manager isn't supported on Windows OS.")
endif(${WIN32})

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++17 -Wall")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")        ## Optimize
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-s")  ## Strip binary
endif()

# for multi-config builds (e.g. msvc)
foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUT_CONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${SC_BIN_PATH})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${SC_BIN_PATH})
endforeach(OUTPUT_CONFIG)

add_subdirectory(src)

if(${SC_BUILD_TESTS})
    add_subdirectory(thirdparty)
    include(tests/tests.cmake)
endif()
