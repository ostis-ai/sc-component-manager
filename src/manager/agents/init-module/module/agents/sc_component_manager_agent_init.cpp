#include <sc-agents-common/utils/AgentUtils.hpp>
#include "utils/sc_component_manager_command_init.hpp"

#include "sc_component_manager_agent_init.hpp"

using namespace initModule;
using namespace keynodes;

SC_AGENT_IMPLEMENTATION(ScComponentManagerInitAgent)
{
  keynodes::ScComponentManagerKeynodes::InitGlobal();
  ScAddr const & actionAddr = otherAddr;

  if (!CheckAction(actionAddr))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_DEBUG("ScComponentManagerInitAgent started");

  //  TODO: change config_path
  std::string const config_path = "/home/anna/projects/ostis-metasystem/sc-models/knowledge-base";

  ScComponentManagerCommandInit command = ScComponentManagerCommandInit(config_path);
  ScAddrVector components = command.Execute(&m_memoryCtx, {});

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, components, true);

  SC_LOG_DEBUG("ScComponentManagerInitAgent finished");
  return SC_RESULT_OK;
};

bool ScComponentManagerInitAgent::CheckAction(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      keynodes::ScComponentManagerKeynodes::action_components_init, actionAddr, ScType::EdgeAccessConstPosPerm);
}
