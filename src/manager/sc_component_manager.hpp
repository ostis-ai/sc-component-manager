/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <thread>
#include <utility>
#include "handler/sc_component_manager_command_handler.hpp"
#include "sc_memory_config.hpp"

class ScComponentManager
{
public:
  explicit ScComponentManager(std::string reposPath, sc_memory_params memoryParams)
    : m_reposPath(std::move(reposPath))
  {
  }

  void Run();

  void Stop();

  virtual ~ScComponentManager() = default;

protected:
  std::string m_reposPath;

  void Start();

  virtual ExecutionResult Emit(std::string const & command) = 0;

  virtual void DisplayResult(ExecutionResult const & executionResult) = 0;

  ScComponentManagerCommandHandler m_handler;

private:
  std::thread m_instance;
};
