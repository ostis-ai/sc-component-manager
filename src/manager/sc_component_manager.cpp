/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <string>
#include <iostream>
#include <thread>

#include "sc_component_manager.hpp"
#include "sc-memory/sc_debug.hpp"
#include "src/manager/commands/sc_component_manager_command.hpp"

void ScComponentManager::Run()
{
  m_instance = std::thread(&ScComponentManager::Start, this);
  m_isRunning = SC_TRUE;
}

void ScComponentManager::Start()
{
  std::string const EXIT_COMMAND = "exit";
  std::string command;
  do
  {
    std::cout << "ScComponentManager: Enter command\n";
    getline(std::cin, command);

    try
    {
      ExecutionResult executionResult = Emit(command);
      DisplayResult(executionResult);
    }
    catch (utils::ScException const & exception)
    {
      std::cout << exception.Description();
      SC_LOG_ERROR(exception.Description());
    }
  } while (m_isRunning && command != EXIT_COMMAND && !command.empty());
}

void ScComponentManager::Stop()
{
  m_isRunning = SC_FALSE;
  if (m_instance.joinable())
    m_instance.detach();
}
