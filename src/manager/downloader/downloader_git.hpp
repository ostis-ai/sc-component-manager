/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>

#include "sc-memory/utils/sc_exec.hpp"
#include "sc-core/sc-store/sc-fs-memory/sc_file_system.h"

#include "downloader.hpp"
#include "src/manager/commands/constants/command_constants.hpp"

class DownloaderGit : public Downloader
{
public:
  // TODO(MksmOrlov): Make a model of github repository (fields username, repository name, default branch).
  // No need to support because of ostis-library.
  bool DownloadFile(std::string const & downloadPath, std::string const & urlAddress, std::string const & pathPostfix)
      override
  {
    std::stringstream query;
    size_t const usernameRepositoryNamePosition =
        urlAddress.find(GitHubConstants::GITHUB_PREFIX) + GitHubConstants::GITHUB_PREFIX.size();
    if (usernameRepositoryNamePosition == std::string::npos)
    {
      return false;
    }
    std::string const componentUsernameRepositoryName = urlAddress.substr(usernameRepositoryNamePosition);

    size_t const usernameRepositoryNameDelimiter =
        componentUsernameRepositoryName.find(SpecificationConstants::DIRECTORY_DELIMITER);
    if (usernameRepositoryNameDelimiter == std::string::npos)
    {
      return false;
    }
    std::string const username = componentUsernameRepositoryName.substr(0, usernameRepositoryNameDelimiter);
    std::string const repositoryName = componentUsernameRepositoryName.substr(usernameRepositoryNameDelimiter + 1);
    std::string const defaultBranch = GetDefaultBranch(username, repositoryName);

    query << GitHubConstants::GITHUB_CURL_COMMAND << downloadPath << SpecificationConstants::DIRECTORY_DELIMITER
          << pathPostfix << " " << GitHubConstants::RAW_GITHUB_PREFIX << componentUsernameRepositoryName
          << SpecificationConstants::DIRECTORY_DELIMITER << defaultBranch << SpecificationConstants::DIRECTORY_DELIMITER
          << pathPostfix;

    if (!sc_fs_create_directory(downloadPath.c_str()))
    {
      SC_LOG_ERROR("Can't download. Can't create folder. " << downloadPath);
      return false;
    }

    ScExec exec{{query.str()}};
    return true;
  }

  static std::string GetDefaultBranch(std::string const & username, std::string const & repositoryName)
  {
    std::stringstream query;
    query << "curl -s -H \"Accept: application/vnd.github.v3+json\" https://api.github.com/repos/" << username
          << SpecificationConstants::DIRECTORY_DELIMITER << repositoryName
          << R"( | grep -w "default_branch" | cut -d ":" -f 2 | tr -d '," ')";

    ScExec exec{{query.str()}};
    std::string const branches(std::istreambuf_iterator<char>(exec), {});
    size_t const actualBranchIndex = branches.find('\n');

    return branches.substr(0, actualBranchIndex);
  }

  bool DownloadRepository(std::string const & downloadPath, std::string const & urlAddress) override
  {
    std::stringstream query;
    query << "cd " << downloadPath << " && " << GitHubConstants::GIT_CLONE << " " << urlAddress;

    if (!sc_fs_create_directory(downloadPath.c_str()))
    {
      SC_LOG_ERROR("Can't download. Can't create folder. " << downloadPath);
      return false;
    }

    ScExec exec{{query.str()}};
    return true;
  }
};
