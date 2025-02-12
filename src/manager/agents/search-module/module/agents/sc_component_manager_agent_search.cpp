/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_component_manager_agent_search.hpp"

#include <common/sc_component_manager_keynodes.hpp>
#include <common/common_utils.hpp>

#include "utils/sc_component_manager_command_search.hpp"

using namespace searchModule;
using namespace keynodes;

ScResult ScComponentManagerSearchAgent::DoProgram(ScAction & action)
{
  ScComponentManagerCommandSearch command = ScComponentManagerCommandSearch();
  ScAddrUnorderedSet components = command.Execute(&m_context, action);
  ScStructure result = m_context.GenerateStructure();
  for (auto const & component : components)
    result << component;
  action.SetResult(result);

  return action.FinishSuccessfully();
}

ScAddr ScComponentManagerSearchAgent::GetActionClass() const
{
  return keynodes::ScComponentManagerKeynodes::action_components_search;
}
