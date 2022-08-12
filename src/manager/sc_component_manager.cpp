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

void ScComponentManager::Run()
{
  m_instance = std::thread(&ScComponentManager::Start, this);
}

void ScComponentManager::Start()
{
  std::string command;
  do
  {
    std::cout << "ScComponentManager: Enter command\n";
    SC_LOG_DEBUG("ScComponentManager: Enter command");
    getline(std::cin, command);

    try
    {
      ExecutionResult executionResult = Emit(command);
      DisplayResult(executionResult);
    }
    catch (utils::ScException const & exception)
    {
      SC_LOG_ERROR(exception.Description());
    }
  } while (command != "exit" && !command.empty());
}

void ScComponentManager::Stop()
{
  if (m_instance.joinable())
    m_instance.join();
}
