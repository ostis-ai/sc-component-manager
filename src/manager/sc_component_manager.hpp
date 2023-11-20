/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <thread>
#include <atomic>
#include <utility>

#include "sc-memory/sc_debug.hpp"
#include "sc_memory_config.hpp"

#include "commands/sc_component_manager_command_handler.hpp"

class ScComponentManager
{
public:
  explicit ScComponentManager(std::map<ScAddr, std::string, ScAddrLessFunc> componentsPath, sc_memory_params memoryParams)
    : m_componentsPath(std::move(componentsPath))
  {
    m_handler = new ScComponentManagerCommandHandler(m_componentsPath);
  }

  void QuietInstall();

  void Run();

  virtual bool Emit(std::string const & command) = 0;

  void Stop();

  virtual ~ScComponentManager()
  {
    delete m_handler;
    m_handler = nullptr;
    ScMemory::Shutdown();
  }

protected:
  std::map<ScAddr, std::string, ScAddLessFunc> m_componentsPath;
  void Start();

  ScComponentManagerCommandHandler * m_handler;

private:
  std::thread m_instance;
  std::atomic<sc_bool> m_isRunning;
};
