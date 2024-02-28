#include "commands_module.hpp"

#include "keynodes/commands_keynodes.hpp"
#include "agents/sc_component_manager_agent_init.hpp"

using namespace commandsModule;

SC_IMPLEMENT_MODULE(CommandsModule)

sc_result CommandsModule::InitializeImpl()
{
  if (!CommandsModule::InitGlobal())
  {
    SC_LOG_ERROR("CommandsModule is deactivated");
    return SC_RESULT_ERROR;
  }

  if (!CommandsKeynodes::InitGlobal())
  {
    SC_LOG_ERROR("CommandsKeynodes is deactivated");
    return SC_RESULT_ERROR;
  }
  else
  {
    SC_AGENT_REGISTER(ScComponentManagerInitAgent);
  }

  return SC_RESULT_OK;
}
sc_result CommandsModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(ScComponentManagerInitAgent);

  return SC_RESULT_OK;
}
