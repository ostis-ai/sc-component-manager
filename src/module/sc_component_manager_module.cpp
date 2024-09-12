/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_component_manager_module.hpp"

SC_MODULE_REGISTER(ScComponentManagerModule);

void ScComponentManagerModule::Initialize(ScMemoryContext * context)
{
  std::string const KB_COMPONENTS_PATH = "knowledge_base_components_path";
  std::string const PS_COMPONENTS_PATH = "problem_solver_components_path";
  std::string const INTERFACE_COMPONENTS_PATH = "interface_components_path";

  // It is backward compatible logic. We should configure platform-dependent components from kb
  ScConfig config{
      ScMemory::ms_configPath,
      {KB_COMPONENTS_PATH, PS_COMPONENTS_PATH, INTERFACE_COMPONENTS_PATH, "repo_path", "extensions_path", "log_file"}};
  ScConfigGroup managerConfig = config["sc-component-manager"];
  for (std::string const & key : *managerConfig)
    m_params.Insert({key, managerConfig[key]});

  std::map<ScAddr, std::string, ScAddrLessFunc> const & componentsPath = {
      {{keynodes::ScComponentManagerKeynodes::concept_reusable_kb_component,
        m_params.Get<std::string>(KB_COMPONENTS_PATH)},
       {keynodes::ScComponentManagerKeynodes::concept_reusable_ps_component,
        m_params.Get<std::string>(PS_COMPONENTS_PATH)},
       {keynodes::ScComponentManagerKeynodes::concept_reusable_ui_component,
        m_params.Get<std::string>(INTERFACE_COMPONENTS_PATH)}}};

  try
  {
    m_scComponentManager = std::make_unique<ScComponentManager>(componentsPath);
    if (!m_scComponentManager)
      return;

    m_scComponentManager->Run();
    SC_LOG_INFO("[sc-component-manager] Sc-component-manager run");
  }
  catch (utils::ScException const & exception)
  {
    SC_LOG_ERROR("[sc-component-manager] " << exception.Description());
    ScComponentManagerModule::Shutdown(context);
  }
}

void ScComponentManagerModule::Shutdown(ScMemoryContext * context)
{
  if (m_scComponentManager)
    m_scComponentManager->Stop();
  SC_LOG_INFO("[sc-component-manager] Sc-component-manager stopped");
  m_scComponentManager.reset();
}
