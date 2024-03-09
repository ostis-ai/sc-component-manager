#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "../../manager/commands/keynodes/ScComponentManagerKeynodes.hpp"
#include "manager/commands/command_init/sc_component_manager_command_init.hpp"

#include "keynodes/commands_keynodes.hpp"
#include "sc_component_manager_agent_init.hpp"

using namespace commandsModule;
using namespace keynodes;

SC_AGENT_IMPLEMENTATION(ScComponentManagerInitAgent)
{
  ScAddr const & actionAddr = otherAddr;

  if (!checkAction(actionAddr))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_DEBUG("ScComponentManagerInitAgent started");
  keynodes::ScComponentManagerKeynodes::InitGlobal();
  // TODO: change config_path
  std::string const config_path = "/home/anna/projects/ostis-metasystem/sc-models/knowledge-base";
  ScComponentManagerCommandInit command = ScComponentManagerCommandInit(config_path);
  command.Execute(&m_memoryCtx, {});

  ScAddrVector components;
  ScIterator3Ptr const & componentsIterator = m_memoryCtx.Iterator3(
      keynodes::ScComponentManagerKeynodes::concept_reusable_component,
      ScType::EdgeAccessConstPosPerm,
      ScType::NodeConst);
  while (componentsIterator->Next())
  {
    components.push_back(componentsIterator->Get(2));
  }
  if (!components.empty())
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, components, true);
  else
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, true);

  SC_LOG_DEBUG("ScComponentManagerInitAgent finished");
  return SC_RESULT_OK;
};

bool ScComponentManagerInitAgent::checkAction(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      commandsModule::CommandsKeynodes::action_components_init, actionAddr, ScType::EdgeAccessConstPosPerm);
}
