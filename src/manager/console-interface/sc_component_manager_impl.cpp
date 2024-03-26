/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_component_manager_impl.hpp"
#include "command_parser/sc_component_manager_command_parser.hpp"

bool ScComponentManagerImpl::Emit(std::string const & command)
{
  std::pair<std::string, CommandParameters> parsed = ScComponentManagerParser::Parse(command);
  bool executionResult = m_handler->Handle(parsed.first, parsed.second);

  std::string log_message = executionResult ? "successfully" : "unsuccessfully";

  SC_LOG_DEBUG("ScComponentManagerImpl: command executed " + log_message);

  return executionResult;
}
