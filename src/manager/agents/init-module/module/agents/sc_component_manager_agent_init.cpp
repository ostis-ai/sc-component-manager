#include <sc-agents-common/utils/AgentUtils.hpp>
#include "utils/sc_component_manager_command_init.hpp"

#include "sc_component_manager_agent_init.hpp"

#include "sc-config-utils/sc-config/sc_config.hpp"

using namespace initModule;
using namespace keynodes;

SC_AGENT_IMPLEMENTATION(ScComponentManagerInitAgent)
{
  ScAddr const & actionAddr = otherAddr;

  if (!CheckAction(actionAddr))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_DEBUG("ScComponentManagerInitAgent started");

  ScConfig config{ScMemory::ms_configPath, {"knowledge_base_components_path"}};
  ScConfigGroup configManager = config["sc-component-manager"];

  ScComponentManagerCommandInit command =
      ScComponentManagerCommandInit(configManager["knowledge_base_components_path"]);

  ScAddrVector components = command.Execute(&m_memoryCtx, actionAddr);

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, components, true);

  SC_LOG_DEBUG("ScComponentManagerInitAgent finished");
  return SC_RESULT_OK;
};

bool ScComponentManagerInitAgent::CheckAction(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      keynodes::ScComponentManagerKeynodes::action_components_init, actionAddr, ScType::EdgeAccessConstPosPerm);
}
