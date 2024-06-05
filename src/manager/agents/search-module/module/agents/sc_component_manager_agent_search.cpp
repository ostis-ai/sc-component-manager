/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-agents-common/utils/AgentUtils.hpp>

#include "module/keynodes/ScComponentManagerKeynodes.hpp"
#include "module/utils/common_utils.hpp"

#include "utils/sc_component_manager_command_search.hpp"

#include "sc_component_manager_agent_search.hpp"

using namespace searchModule;
using namespace keynodes;

SC_AGENT_IMPLEMENTATION(ScComponentManagerSearchAgent)
{
  ScAddr const & actionAddr = otherAddr;

  if (!CheckAction(m_memoryCtx, actionAddr, keynodes::ScComponentManagerKeynodes::action_components_search))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_DEBUG("ScComponentManagerSearchAgent started");

  ScComponentManagerCommandSearch command = ScComponentManagerCommandSearch();
  common_utils::CommonUtils::ScAddrUnorderedSet components = command.Execute(&m_memoryCtx, actionAddr);
  ScAddrVector result(components.begin(), components.end());
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, result, true);

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
