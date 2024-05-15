/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_component_manager_module.hpp"

#include "sc-component-manager-factory/sc_component_manager_factory.hpp"

SC_IMPLEMENT_MODULE(ScComponentManagerModule)

sc_result ScComponentManagerModule::InitializeImpl()
{
  // It is backward compatible logic. When all platform-dependent components will be configured from kb it will be
  // removed.
  ScConfig config{
      ScMemory::ms_configPath,
      {"knowledge_base_components_path",
       "problem_solver_components_path",
       "interface_components_path",
       "repo_path",
       "extensions_path",
       "log_file"}};
  ScConfigGroup managerConfig = config["sc-component-manager"];
  for (auto const & key : *managerConfig)
    m_params.Insert({key, managerConfig[key]});

  try
  {
    m_scComponentManager = ScComponentManagerFactory::ConfigureScComponentManager(m_params);
    if (!m_scComponentManager)
      return SC_RESULT_ERROR;

    m_scComponentManager->Run();
    SC_LOG_INFO("[sc-component-manager] Sc-component-manager run");
  }
  catch (utils::ScException const & exception)
  {
    SC_LOG_ERROR("[sc-component-manager] " << exception.Description());
    ScComponentManagerModule::ShutdownImpl();

    return SC_RESULT_ERROR;
  }

  return SC_RESULT_OK;
}

sc_result ScComponentManagerModule::ShutdownImpl()
{
  if (m_scComponentManager)
    m_scComponentManager->Stop();
  SC_LOG_INFO("[sc-component-manager] Sc-component-manager stopped");
  m_scComponentManager.reset();

  return SC_RESULT_OK;
}
