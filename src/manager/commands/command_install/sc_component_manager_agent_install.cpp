#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"

#include "manager/commands/keynodes/ScComponentManagerKeynodes.hpp"
#include "sc_component_manager_agent_install.hpp"
#include "sc_component_manager_command_install.hpp"
using namespace commandsModule;
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
  std::map<ScAddr, std::string, ScAddrLessFunc> config_path;

  config_path.insert({keynodes::ScComponentManagerKeynodes::concept_reusable_kb_component, config_path_kb});
  config_path.insert({keynodes::ScComponentManagerKeynodes::concept_reusable_ps_component, config_path_ps});
  config_path.insert({keynodes::ScComponentManagerKeynodes::concept_reusable_interface_component, config_path_ui});

  ScComponentManagerCommandInstall command = ScComponentManagerCommandInstall(config_path);
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

ScAddrVector ScComponentManagerInstallAgent::GetParameterNodeUnderRelation(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & actionAddr,
    ScAddr const & relation)
{
  ScAddr parameterNode;
  ScAddrVector components;
  ScIterator5Ptr const & parameterIterator = m_memoryCtx.Iterator5(
      actionAddr, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, relation);
  if (parameterIterator->Next())
  {
    parameterNode = parameterIterator->Get(2);
    ScIterator3Ptr const & componentsIterator =
        m_memoryCtx.Iterator3(parameterNode, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (componentsIterator->Next())
    {
      components.push_back(componentsIterator->Get(2));
    }
  }
  return components;
}
