/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "init_module.hpp"

#include "agents/sc_component_manager_agent_init.hpp"

using namespace initModule;

SC_IMPLEMENT_MODULE(InitModule)

sc_result InitModule::InitializeImpl()
{
  if (!InitModule::InitGlobal())
  {
    SC_LOG_ERROR("InitModule is deactivated");
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
