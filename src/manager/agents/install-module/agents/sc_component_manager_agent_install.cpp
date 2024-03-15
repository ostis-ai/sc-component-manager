#include <sc-agents-common/utils/AgentUtils.hpp>

#include "../utils/sc_component_manager_command_install.hpp"

#include "sc_component_manager_agent_install.hpp"

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
  keynodes::ScComponentManagerKeynodes::InitGlobal();

  // TODO: change config_path_**
  std::string const config_path_kb = "/home/anna/projects/ostis-metasystem/sc-models/knowledge-base";
  std::string const config_path_ps =
      "/home/anna/projects/ostis-metasystem/platform-dependent-components/problem-solver";
  std::string const config_path_ui = "/home/anna/projects/ostis-metasystem/platform-dependent-components/interface";
  std::map<ScAddr, std::string, ScAddrLessFunc> componentWithConfigPath;

  componentWithConfigPath.insert({keynodes::ScComponentManagerKeynodes::concept_reusable_kb_component, config_path_kb});
  componentWithConfigPath.insert({keynodes::ScComponentManagerKeynodes::concept_reusable_ps_component, config_path_ps});
  componentWithConfigPath.insert({keynodes::ScComponentManagerKeynodes::concept_reusable_interface_component, config_path_ui});

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
