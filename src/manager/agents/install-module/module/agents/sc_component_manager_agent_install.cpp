/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "utils/sc_component_manager_command_install.hpp"

#include "sc_component_manager_agent_install.hpp"

#include "sc-config-utils/sc-config/sc_config.hpp"

using namespace installModule;
using namespace keynodes;

ScResult ScComponentManagerInstallAgent::DoProgram(ScAction & action)
{
  SC_AGENT_LOG_DEBUG("started");

  std::map<ScAddr, std::string, ScAddrLessFunc> componentWithConfigPath;

  ScConfig config{
      ScMemory::ms_configPath,
      {PathKeysOfConfigPath::KB_PATH, PathKeysOfConfigPath::PS_PATH, PathKeysOfConfigPath::UI_PATH}};
  ScConfigGroup configManager = config["sc-component-manager"];

  componentWithConfigPath.insert(
      {keynodes::ScComponentManagerKeynodes::concept_reusable_kb_component,
       configManager[PathKeysOfConfigPath::KB_PATH]});
  componentWithConfigPath.insert(
      {keynodes::ScComponentManagerKeynodes::concept_reusable_ps_component,
       configManager[PathKeysOfConfigPath::PS_PATH]});
  componentWithConfigPath.insert(
      {keynodes::ScComponentManagerKeynodes::concept_reusable_ui_component,
       configManager[PathKeysOfConfigPath::UI_PATH]});

  ScComponentManagerCommandInstall command = ScComponentManagerCommandInstall(componentWithConfigPath);
  ScAddrUnorderedSet const & identifiersNodesSet = command.Execute(&m_context, action);
  bool isSuccess = !identifiersNodesSet.empty();
  ScStructure result = m_context.GenerateStructure();
  for (auto const & identifierNode : identifiersNodesSet)
    result << identifierNode;
  action.SetResult(result);

  SC_AGENT_LOG_DEBUG("finished");
  return isSuccess ? action.FinishSuccessfully() : action.FinishUnsuccessfully();
}

ScAddr ScComponentManagerInstallAgent::GetActionClass() const
{
  return keynodes::ScComponentManagerKeynodes::action_components_install;
}
