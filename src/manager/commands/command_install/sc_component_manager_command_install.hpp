/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <dirent.h>
#include <sys/stat.h>

#include "src/manager/commands/sc_component_manager_command.hpp"
#include "src/manager/commands/keynodes/ScComponentManagerKeynodes.hpp"

extern "C"
{
#include "sc-core/sc-store/sc-fs-storage/sc_file_system.h"
}

class ScComponentManagerCommandInstall : public ScComponentManagerCommand
{
public:
  explicit ScComponentManagerCommandInstall(std::string specificationsPath);

  ExecutionResult Execute(ScMemoryContext * context, CommandParameters const & commandParameters) override;

protected:
  static std::string GetComponentAddress(ScMemoryContext * context, ScAddr const & componentAddr);

  static ScAddrVector GetComponentDependencies(ScMemoryContext * context, ScAddr const & componentAddr);

  static ScAddr GetComponentInstallationMethod(ScMemoryContext * context, ScAddr const & componentAddr);

  std::string const PARAMETER_NAME = "idtf";

  std::string m_specificationsPath;
};
