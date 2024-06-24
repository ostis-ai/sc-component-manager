/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_component_manager.hpp"

#include <sys/select.h>

#include "sc-memory/sc_debug.hpp"

#include "command_parser/sc_component_manager_command_parser.hpp"
#include "command_handler/sc_component_manager_command_handler.hpp"

static constexpr int STD_INPUT = 0;
static constexpr suseconds_t WAIT_BETWEEN_SELECT_US = 250000L;

void ScComponentManager::Run()
{
  m_isRunning = SC_TRUE;
  m_instance = std::thread(&ScComponentManager::Start, this);
}

void ScComponentManager::Stop()
{
  m_isRunning = SC_FALSE;
  if (m_instance.joinable())
    m_instance.join();
}

void ScComponentManager::Start()
{
  std::string command;
  while (m_isRunning)
  {
    if (HasNewInput())
      std::getline(std::cin, command);

    if (command.empty())
      continue;

    try
    {
      Emit(command);
    }
    catch (utils::ScException const & exception)
    {
      SC_LOG_ERROR(exception.Message());
    }

    command.clear();
  }
}

sc_bool ScComponentManager::HasNewInput()
{
  struct timeval waitTime = {0L, WAIT_BETWEEN_SELECT_US};
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(STD_INPUT, &fds);
  int ready = select(STD_INPUT + 1, &fds, nullptr, nullptr, &waitTime);
  return ready > 0;
}

bool ScComponentManager::Emit(std::string const & command)
{
  std::pair<std::string, CommandParameters> const parsedCommand = ScComponentManagerParser::Parse(command);
  ScComponentManagerCommandHandler handler;
  bool const executionResult = handler.Handle(parsedCommand.first, parsedCommand.second);

  std::string const logMessage = executionResult ? "successfully" : "unsuccessfully";

  SC_LOG_DEBUG("ScComponentManager: command executed " << logMessage);

  return executionResult;
}
