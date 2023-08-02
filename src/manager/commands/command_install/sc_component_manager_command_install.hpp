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
#include "src/manager/downloader/downloader.hpp"
#include "src/manager/downloader/downloader_handler.hpp"

extern "C"
{
#include "sc-core/sc-store/sc-fs-memory/sc_file_system.h"
}

class ScComponentManagerCommandInstall : public ScComponentManagerCommand
{
  std::string const PARAMETER_NAME = "idtf";

public:
  explicit ScComponentManagerCommandInstall(std::map<ScAddr, std::string, ScAddLessFunc> m_componentsPath);

  bool Execute(ScMemoryContext * context, CommandParameters const & commandParameters) override;

protected:
  static void ValidateComponent(ScMemoryContext * context, ScAddr const & componentAddr);

  bool DownloadComponent(ScMemoryContext * context, ScAddr const & componentAddr);

  bool InstallDependencies(ScMemoryContext * context, ScAddr const & componentAddr);

  ScAddrVector GetAvailableComponents(ScMemoryContext * context, std::vector<std::string> componentsToInstall);

  bool InstallComponent(ScMemoryContext * context, ScAddr const & componentAddr);

  // Install components that are in `need to install` set
  void QuietInstall(ScMemoryContext * context);

  std::map<ScAddr, std::string, ScAddLessFunc> m_componentsPath;

  std::string m_downloadDir;

  std::unique_ptr<DownloaderHandler> downloaderHandler;
};
