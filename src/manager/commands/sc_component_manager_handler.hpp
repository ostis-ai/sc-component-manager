/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <utility>

#include "sc_component_manager_command.hpp"

class ScComponentManagerHandler
{
public:
  virtual bool Handle(std::string const & commandType, CommandParameters const & commandParameters) = 0;

  virtual ~ScComponentManagerHandler() = default;
};
