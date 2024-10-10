/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <dirent.h>
#include <sys/stat.h>

#include "module/utils/common_utils.hpp"
#include "module/keynodes/ScComponentManagerKeynodes.hpp"

#include "commands/sc_component_manager_command.hpp"

#include "downloader/downloader.hpp"
#include "downloader/downloader_handler.hpp"

class ScComponentManagerCommandInstall : public ScComponentManagerCommand
{
  std::string const PARAMETER_NAME = "idtf";

public:
  explicit ScComponentManagerCommandInstall(std::map<ScAddr, std::string, ScAddrLessFunc> m_componentsPath);

  ScAddrUnorderedSet Execute(ScAgentContext * context, ScAddr const & actionAddr) override;

protected:
  static void ValidateComponent(ScMemoryContext * context, ScAddr const & componentAddr);

  bool DownloadComponent(ScMemoryContext * context, ScAddr const & componentAddr);

  bool InstallDependencies(ScAgentContext * context, ScAddr const & componentAddr);

  static ScAddr CreateSetToInstallStructure(ScMemoryContext * context, ScAddr const & dependenciesSet);

  static ScAddr CheckDependencyDuplication(
      ScMemoryContext * context,
      ScAddr const & currentInstallationComponentsAddr,
      ScAddr const & dependenciesSet);

  static bool EraseTempOutputEdges(ScMemoryContext * context, ScAddr const & node);

  static ScAddrUnorderedSet GetAvailableComponents(
      ScMemoryContext * context,
      ScAddrUnorderedSet const & componentsToInstallIdentifiers);

  bool InstallComponent(ScMemoryContext * context, ScAddr const & componentAddr);

  // Install components that are in `need to install` set
  void QuietInstall(ScMemoryContext * context);

  std::map<ScAddr, std::string, ScAddrLessFunc> m_componentsPath;

  std::string m_downloadDir;

  std::unique_ptr<DownloaderHandler> downloaderHandler;
};
