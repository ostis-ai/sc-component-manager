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
#include "sc-config-utils/sc_memory_config.hpp"

#include "sc_component_manager_command_handler.hpp"

class ScComponentManager
{
public:
  explicit ScComponentManager() = default;

  void Run();

  void Stop();

  virtual ~ScComponentManager() = default;

protected:
  void Start();

  sc_bool static HasNewInput();

  bool Emit(std::string const & command);

private:
  std::thread m_instance;
  std::atomic<sc_bool> m_isRunning{};
};
