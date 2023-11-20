/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <utility>

#include "sc_component_manager.hpp"

class ScComponentManagerImpl : public ScComponentManager
{
public:
  ScComponentManagerImpl(std::map<ScAddr, std::string, ScAddLessFunc> componentsPath, sc_memory_params memoryParams)
    : ScComponentManager(std::move(componentsPath), memoryParams)
  {
  }

  ~ScComponentManagerImpl() override = default;

protected:
  bool Emit(std::string const & command) override;
};
