/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <thread>
#include <atomic>
#include <map>

#include "sc-memory/sc_addr.hpp"

class ScComponentManager
{
public:
  ScComponentManager() = default;

  /*!
   * @brief Creates new thread instance for sc-component-manager
   */
  void Run();

  /*!
   * @brief Stor sc-component-manager. Join its thread instance
   */
  void Stop();

  ~ScComponentManager() = default;

protected:
  /*!
   * @brief Get command from terminal input and emit command
   */
  void Start();

  /*!
   * @brief Check if terminal has new input to emit command
   * @return true if there is new input, otherwise false
   */
  sc_bool static HasNewInput();

  /*!
   * @brief Parse command into type and parameters with values then handle command
   * @param command raw string of command
   * @return execution result of the emitted command
   */
  static bool Emit(std::string const & command);

private:
  std::thread m_instance;              /// New thread for sc-component-manager commands handling
  std::atomic<sc_bool> m_isRunning{};  /// Flag to stop sc-component-manager commands handling
};
