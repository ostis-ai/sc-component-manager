#include <sc-agents-common/utils/AgentUtils.hpp>
#include "keynodes/ScComponentManagerKeynodes.hpp"
#include "utils/sc_component_manager_command_search.hpp"

#include "sc_component_manager_agent_search.hpp"

using namespace searchModule;
using namespace keynodes;

SC_AGENT_IMPLEMENTATION(ScComponentManagerSearchAgent)
{
  ScAddr const & actionAddr = otherAddr;

  keynodes::ScComponentManagerKeynodes::InitGlobal();
  if (!CheckAction(m_memoryCtx, actionAddr, keynodes::ScComponentManagerKeynodes::action_components_search))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_DEBUG("ScComponentManagerSearchAgent started");
  keynodes::ScComponentManagerKeynodes::InitGlobal();

  ScComponentManagerCommandSearch command = ScComponentManagerCommandSearch();
  ScAddrVector components = command.Execute(&m_memoryCtx, actionAddr);

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, components, true);

  SC_LOG_DEBUG("ScComponentManagerSearchAgent finished");
  return SC_RESULT_OK;
}

bool ScComponentManagerSearchAgent::CheckAction(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & actionAddr,
    ScAddr const & actionAddrClass)
{
  return m_memoryCtx.HelperCheckEdge(actionAddrClass, actionAddr, ScType::EdgeAccessConstPosPerm);
}
