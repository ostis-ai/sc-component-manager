/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "install-module.hpp"

#include "agents/sc_component_manager_agent_install.hpp"

using namespace installModule;

SC_IMPLEMENT_MODULE(InstallModule)

sc_result InstallModule::InitializeImpl()
{
  if (!InstallModule::InitGlobal())
  {
    SC_LOG_ERROR("InstallModule is deactivated");
    return SC_RESULT_ERROR;
  }

  SC_AGENT_REGISTER(ScComponentManagerInstallAgent);

  return SC_RESULT_OK;
}

sc_result InstallModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(ScComponentManagerInstallAgent);

  return SC_RESULT_OK;
}
