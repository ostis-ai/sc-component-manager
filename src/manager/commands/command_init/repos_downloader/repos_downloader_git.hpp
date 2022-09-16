/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>

#include "repos_downloader.hpp"
#include "src/manager/commands/command_init/constants/command_init_constants.hpp"

class ReposDownloaderGit : public ReposDownloader
{
public:
  void Download(std::string const & componentPath, std::string const & specificationsPath, bool is_repository) override
  {
    std::string pathPostfix = is_repository ? "/" + SpecificationConstants::REPOS_FILENAME
                                            : "/" + SpecificationConstants::SPECIFICATION_FILENAME;
    std::string const svnInstallCommand =
        "cd " + specificationsPath + " ; svn export " + componentPath + GitHubConstants::SVN_TRUNK + pathPostfix;

    system(svnInstallCommand.c_str());
  }
};
