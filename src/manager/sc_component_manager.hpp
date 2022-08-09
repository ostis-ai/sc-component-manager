/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <thread>

class ScComponentManager
{
public:
  void Run();

  void Stop();

protected:
  void Start();

  virtual void Emit(std::string const & command) = 0;

private:
  std::thread m_instance;
};
