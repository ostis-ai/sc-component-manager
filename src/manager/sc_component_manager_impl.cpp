/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_component_manager_impl.hpp"
#include "command_parser/sc_component_manager_command_parser.hpp"

ExecutionResult ScComponentManagerImpl::Emit(std::string const & command)
{
  std::pair<std::string, CommandParameters> parsed = ScComponentManagerParser::Parse(command);
  ExecutionResult executionResult = m_handler->Handle(parsed.first, parsed.second);

  SC_LOG_DEBUG("ScComponentManagerImpl: execution result size is " + std::to_string(executionResult.size()));
  // call formatter to format result

  return executionResult;
}

void ScComponentManagerImpl::DisplayResult(ExecutionResult const & executionResult)
{
  for (std::string const & resultItem : executionResult)
  {
    SC_LOG_INFO("ScComponentManager: " + resultItem);
  }
}
