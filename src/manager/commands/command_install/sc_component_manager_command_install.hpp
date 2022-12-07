/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "src/manager/commands/sc_component_manager_command.hpp"
#include "src/manager/commands/keynodes/ScComponentManagerKeynodes.hpp"

class ScComponentManagerCommandInstall : public ScComponentManagerCommand
{
public:
  ScComponentManagerCommandInstall() = default;

  ExecutionResult Execute(ScMemoryContext * context, CommandParameters const & commandParameters) override;

protected:
  std::string const PARAMETER_NAME = "idtf";

};
