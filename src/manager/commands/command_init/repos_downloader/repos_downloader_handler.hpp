/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>
#include <map>

extern "C"
{
#include "sc-core/sc-store/sc-fs-storage/sc_file_system.h"
}

#include "sc-memory/sc_scs_helper.hpp"

#include "repos_downloader.hpp"
#include "repos_downloader_git.hpp"
#include "repos_downloader_google_drive.hpp"
#include "src/manager/commands/keynodes/ScComponentManagerKeynodes.hpp"

class ReposDownloaderHandler
{
public:
  // void HandleComponents(ScMemoryContext * context, std::string const & componentPath, std::string &
  // specificationsPath);

  // void HandleRepositories(std::string const & repositoryPath, std::string & specificationsPath);

  ~ReposDownloaderHandler();

protected:
  static char const DIRECTORY_DELIMITER = '/';
  std::map<ScAddr, ReposDownloader *, ScAddrLessFunc> m_downloaders = {
      {keynodes::ScComponentManagerKeynodes::concept_github_url, new ReposDownloaderGit()},
      {keynodes::ScComponentManagerKeynodes::concept_google_drive_url, new ReposDownloaderGoogleDrive()}};

  // static void Download(
  //     std::string const & path,
  //     std::string & specificationsPath,
  //     ReposDownloader * downloader,
  //     bool isRepository);

  static void Download(ScMemoryContext * context, ScAddr const & nodeAddr);

  // static std::string GetSpecificationDirName(std::string const & componentPath, std::string & specificationsPath);
};
