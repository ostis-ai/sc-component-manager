/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>

#include "sc-memory/utils/sc_exec.hpp"

#include "downloader.hpp"
#include "src/manager/commands/command_init/constants/command_init_constants.hpp"

class DownloaderGit : public Downloader
{
public:
  void Download(std::string const & downloadPath, std::string const & urlAddress, std::string const & pathPostfix = "")
      override
  {
    ScExec exec{
        {"cd",
         downloadPath,
         "&&",
         "svn",
         "export",
         urlAddress + GitHubConstants::SVN_TRUNK + SpecificationConstants::DIRECTORY_DELIMETR + pathPostfix}};
  }
};
