macro(sc_target_dependencies)
    include(CTest)

    if(${UNIX})
        sc_linux_target_dependencies()
    elseif(${WIN32})
        message(SEND_ERROR "sc-component-manager isn't supported on Windows OS.")
    endif(${UNIX})
endmacro()

macro(sc_linux_target_dependencies)
    set(SC_CODEGEN_TOOL "${SC_BIN_PATH}/sc-code-generator")
    # for std::thread support
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endmacro()
