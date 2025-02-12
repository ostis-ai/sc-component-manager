/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>
#include <map>

#include <sc-memory/sc_scs_helper.hpp>

#include "common/sc_component_manager_keynodes.hpp"
#include "common/downloader.hpp"
#include "common/downloader_git.hpp"
#include "common/downloader_google_drive.hpp"

class DownloaderHandler
{
public:
  explicit DownloaderHandler(std::string specificationsPath)
    : m_downloadDir(std::move(specificationsPath))
  {
    m_downloader = std::make_shared<DownloaderGit>();
  }

  bool DownloadSpecification(ScMemoryContext * context, ScAddr const & componentSpecificationAddr);
  bool DownloadComponent(ScMemoryContext * context, ScAddr const & nodeAddr);

  void setDownloadDir(std::string const & downloadDir);

protected:
  std::string m_downloadDir;
  std::shared_ptr<Downloader> m_downloader;

  static ScAddr getDownloadableClass(ScMemoryContext * context, ScAddr const & nodeAddr);
  static ScAddr getUrlLinkClass(ScMemoryContext * context, ScAddr const & linkAddr);
};
