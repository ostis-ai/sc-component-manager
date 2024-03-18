#include <sc-agents-common/utils/AgentUtils.hpp>

#include "../utils/sc_component_manager_command_install.hpp"

#include "sc_component_manager_agent_install.hpp"

#include "sc-config-utils/sc-config/sc_config.hpp"

using namespace installModule;
using namespace keynodes;

SC_AGENT_IMPLEMENTATION(ScComponentManagerInstallAgent)
{
  ScAddr const & actionAddr = otherAddr;

  keynodes::ScComponentManagerKeynodes::InitGlobal();
  if (!CheckAction(actionAddr))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_DEBUG("ScComponentManagerInstallAgent started");

  std::map<ScAddr, std::string, ScAddrLessFunc> componentWithConfigPath;

  ScConfig config{
      ScMemory::ms_configPath,
      {"knowledge_base_components_path", "problem_solver_components_path", "interface_components_path"}};
  ScConfigGroup configManager = config["sc-component-manager"];

  componentWithConfigPath.insert(
      {keynodes::ScComponentManagerKeynodes::concept_reusable_kb_component,
       configManager["knowledge_base_components_path"]});
  componentWithConfigPath.insert(
      {keynodes::ScComponentManagerKeynodes::concept_reusable_ps_component,
       configManager["problem_solver_components_path"]});
  componentWithConfigPath.insert(
      {keynodes::ScComponentManagerKeynodes::concept_reusable_interface_component,
       configManager["interface_components_path"]});

  ScComponentManagerCommandInstall command = ScComponentManagerCommandInstall(componentWithConfigPath);
  ScAddrVector identifiersNodes = command.Execute(&m_memoryCtx, actionAddr);

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, identifiersNodes, true);

  SC_LOG_DEBUG("ScComponentManagerInstallAgent finished");
  return SC_RESULT_OK;
};

bool ScComponentManagerInstallAgent::CheckAction(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      keynodes::ScComponentManagerKeynodes::action_components_install, actionAddr, ScType::EdgeAccessConstPosPerm);
}
