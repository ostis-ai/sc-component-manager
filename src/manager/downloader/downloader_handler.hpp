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
#include "sc-core/sc-store/sc-fs-memory/sc_file_system.h"
}

#include "sc-memory/sc_scs_helper.hpp"

#include "downloader.hpp"
#include "downloader_git.hpp"
#include "downloader_google_drive.hpp"
#include "src/manager/commands/keynodes/ScComponentManagerKeynodes.hpp"

class DownloaderHandler
{
public:
  explicit DownloaderHandler(std::string specificationsPath)
    : m_downloadDir(std::move(specificationsPath))
  {
  }

  DownloaderHandler() = default;

  ~DownloaderHandler();

  bool Download(ScMemoryContext * context, ScAddr const & nodeAddr);

  void setDownloadDir(std::string const & downloadDir);

protected:
  std::string m_downloadDir;
  static char const DIRECTORY_DELIMITER = '/';
  std::map<ScAddr, Downloader *, ScAddrLessFunc> m_downloaders = {
      {keynodes::ScComponentManagerKeynodes::concept_github_url, new DownloaderGit()},
      {keynodes::ScComponentManagerKeynodes::concept_google_drive_url, new DownloaderGoogleDrive()}};

  ScAddr getDownloadableClass(ScMemoryContext * context, ScAddr const & nodeAddr);
  ScAddr getUrlLinkClass(ScMemoryContext * context, ScAddr const & linkAddr);
};
