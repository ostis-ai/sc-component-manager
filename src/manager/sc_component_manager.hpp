/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <thread>
#include <atomic>
#include <utility>

#include "sc_memory_config.hpp"

#include "commands/sc_component_manager_command_handler.hpp"

class ScComponentManager
{
public:
  explicit ScComponentManager(std::string reposPath, std::string specificationsPath, sc_memory_params memoryParams)
    : m_reposPath(std::move(reposPath)),
    m_specificationsPath(std::move(specificationsPath))
  {
    ScMemory::Initialize(memoryParams);
    m_handler = new ScComponentManagerCommandHandler(m_reposPath, m_specificationsPath);
  }

  void Run();

  void Stop();

  virtual ~ScComponentManager()
  {
    delete m_handler;
    m_handler = nullptr;
    ScMemory::Shutdown();
  }

protected:
  std::string m_reposPath;
  std::string m_specificationsPath;
  void Start();

  virtual ExecutionResult Emit(std::string const & command) = 0;

  virtual void DisplayResult(ExecutionResult const & executionResult) = 0;

  ScComponentManagerCommandHandler * m_handler;

private:
  std::thread m_instance;
  std::atomic<sc_bool> m_isRunning;
};
