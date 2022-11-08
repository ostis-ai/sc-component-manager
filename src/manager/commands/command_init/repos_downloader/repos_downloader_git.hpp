/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>

#include "sc-memory/utils/sc_exec.hpp"

#include "repos_downloader.hpp"
#include "src/manager/commands/command_init/constants/command_init_constants.hpp"

class ReposDownloaderGit : public ReposDownloader
{
public:
  void Download(std::string const & componentPath, std::string const & specificationsPath, bool isRepository) override
  {
    char const DIRECTORY_DELIMITER = '/';
    std::string pathPostfix = isRepository ? DIRECTORY_DELIMITER + SpecificationConstants::REPOS_FILENAME
                                            : DIRECTORY_DELIMITER + SpecificationConstants::SPECIFICATION_FILENAME;

    ScExec exec{
        {"cd", specificationsPath, "&&", "svn", "export", componentPath + GitHubConstants::SVN_TRUNK + pathPostfix}
    };
  }
};
