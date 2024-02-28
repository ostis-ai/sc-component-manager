set(COMMANDS_MODULE_SRC "${CMAKE_SOURCE_DIR}/commandsModule")

set(SOURCES
    "agents/sc_component_manager_agent_init.cpp"
    "agents/sc_component_manager_agent_init.hpp"
    "keynodes/commands_keynodes.cpp"
    "keynodes/commands_keynodes.hpp"
    "commands_module.cpp"
    "commands_module.hpp"
)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
)

add_library(commands SHARED ${SOURCES})
target_link_libraries(commands sc-component-manager-lib sc-memory sc-agents-common)

sc_codegen_ex(commands ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_CLANG_FORMAT_CODE})
  target_clangformat_setup(commands)
endif()
