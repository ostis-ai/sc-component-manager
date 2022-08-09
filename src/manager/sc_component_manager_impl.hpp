/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc_component_manager.hpp"

#include "sc-memory/sc_memory.hpp"
#include "sc_component.hpp"

class ScComponentManagerImpl : public ScComponentManager
{
protected:
  virtual void Emit(std::string const & command) override;
};
