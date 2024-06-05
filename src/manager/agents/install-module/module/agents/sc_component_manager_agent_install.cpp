/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-agents-common/utils/AgentUtils.hpp>

#include "utils/sc_component_manager_command_install.hpp"

#include "sc_component_manager_agent_install.hpp"

#include "sc-config-utils/sc-config/sc_config.hpp"

using namespace installModule;
using namespace keynodes;

SC_AGENT_IMPLEMENTATION(ScComponentManagerInstallAgent)
{
  ScAddr const & actionAddr = otherAddr;

  if (!CheckAction(actionAddr))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_DEBUG("ScComponentManagerInstallAgent started");

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
  std::unordered_set<ScAddr, ScAddrHashFunc<sc_uint32>> const & identifiersNodes =
      command.Execute(&m_memoryCtx, actionAddr);
  ScAddrVector result(identifiersNodes.begin(), identifiersNodes.end());
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, result, true);

  SC_LOG_DEBUG("ScComponentManagerInstallAgent finished");
  return SC_RESULT_OK;
};

bool ScComponentManagerInstallAgent::CheckAction(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      keynodes::ScComponentManagerKeynodes::action_components_install, actionAddr, ScType::EdgeAccessConstPosPerm);
}
