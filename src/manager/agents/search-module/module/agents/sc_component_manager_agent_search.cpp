/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "module/keynodes/ScComponentManagerKeynodes.hpp"
#include "module/utils/common_utils.hpp"

#include "utils/sc_component_manager_command_search.hpp"

#include "sc_component_manager_agent_search.hpp"

using namespace searchModule;
using namespace keynodes;

ScResult ScComponentManagerSearchAgent::DoProgram(ScAction & action)
{
  SC_AGENT_LOG_DEBUG("started");

  ScComponentManagerCommandSearch command = ScComponentManagerCommandSearch();
  ScAddrUnorderedSet components = command.Execute(&m_context, action);
  ScStructure result = m_context.GenerateStructure();
  for (auto const & component : components)
    result << component;
  action.SetResult(result);

  SC_AGENT_LOG_DEBUG("finished");
  return action.FinishSuccessfully();
}

ScAddr ScComponentManagerSearchAgent::GetActionClass() const
{
  return keynodes::ScComponentManagerKeynodes::action_components_search;
}
