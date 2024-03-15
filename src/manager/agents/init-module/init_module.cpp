#include "init_module.hpp"

#include "agents/sc_component_manager_agent_init.hpp"

using namespace initModule;

SC_IMPLEMENT_MODULE(InitModule)

sc_result InitModule::InitializeImpl()
{
  if (!InitModule::InitGlobal())
  {
    SC_LOG_ERROR("InstallModule is deactivated");
    return SC_RESULT_ERROR;
  }

  SC_AGENT_REGISTER(ScComponentManagerInitAgent);

  return SC_RESULT_OK;
}

sc_result InitModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(ScComponentManagerInitAgent);

  return SC_RESULT_OK;
}
