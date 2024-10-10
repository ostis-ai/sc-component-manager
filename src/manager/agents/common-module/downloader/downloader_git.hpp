/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>
#include <filesystem>

#include "sc-memory/utils/sc_exec.hpp"
#include <filesystem>

#include "downloader.hpp"
#include "constants/command_constants.hpp"
#include "url_parser/repository_url_parser.hpp"

class DownloaderGit : public Downloader
{
public:
  /**
   * @brief Download only one file from the url address
   * Executing the next commands:
   *
   * @param downloadPath path to download component into
   * @param urlAddress component address to install
   * @param pathPostfix filename to download is "specification.scs" by default
   * @return true if downloaded successfully
   */
  bool DownloadFile(std::string const & downloadPath, std::string const & urlAddress, std::string const & pathPostfix)
      override
  {
    std::error_code errorCode;
    if (!std::filesystem::create_directories(downloadPath, errorCode) && errorCode)
    {
      SC_LOG_ERROR(
          "DownloaderGit: Can't download file. Can't create folder " << downloadPath << " with error message "
                                                                     << errorCode.message());
      return false;
    }

    std::stringstream query;

    RepositoryUrlParser parser;
    parser.Parse(urlAddress);
    std::string const username = parser.GetUsername();
    std::string const repositoryName = parser.GetRepositoryName();
    std::string const directoryName = parser.GetDirectoryName();
    std::string const defaultBranch = GetDefaultBranch(username, repositoryName);

    // curl command to download file
    query << GitHubConstants::GITHUB_DOWNLOAD_FILE_COMMAND_PREFIX;
    // path to download file into
    query << downloadPath << SpecificationConstants::DIRECTORY_DELIMITER << pathPostfix << " ";
    // gitHub url to download file from
    query << GitHubConstants::RAW_GITHUB_PREFIX << username << SpecificationConstants::DIRECTORY_DELIMITER
          << repositoryName << SpecificationConstants::DIRECTORY_DELIMITER << defaultBranch
          << SpecificationConstants::DIRECTORY_DELIMITER;
    // add directory if needed
    if (!directoryName.empty())
    {
      query << directoryName << SpecificationConstants::DIRECTORY_DELIMITER;
    }
    // filename to download
    query << pathPostfix;
    ScExec exec{{query.str()}};
    return true;
  }

  /**
   * @brief Download repository from the url address. If it is a subdirectory, download only subdirectory
   * Executing the next commands (directory is optional):
   * git clone https://github.com/username/repo/directory (if there is no subdirectory in the url)
   * otherwise, the next command
   * git clone --no-checkout --depth=1 --filter=tree:0 https://github.com/username/repo/directory
   * cd repo
   * git sparse-checkout set --no-cone directory
   * git checkout
   * @param downloadPath path to download component into
   * @param urlAddress component address to install
   * @return true if downloaded successfully
   */

  bool DownloadRepository(std::string const & downloadPath, std::string const & urlAddress) override
  {
    std::error_code errorCode;
    if (!std::filesystem::create_directories(downloadPath, errorCode) && errorCode)
    {
      SC_LOG_ERROR(
          "DownloaderGit: Can't download repository. Can't create folder " << downloadPath << " with error message "
                                                                           << errorCode.message());
      return false;
    }

    RepositoryUrlParser parser;
    parser.Parse(urlAddress);
    std::string const repositoryAddress = parser.GetRepositoryUrl();
    std::string const repositoryName = parser.GetRepositoryName();
    std::string const directoryName = parser.GetDirectoryName();

    // Form the query to download a repository using git
    std::stringstream query;

    // Navigate to the corresponding component directory
    query << "cd " << downloadPath << " && ";

    if (directoryName.empty())
    {
      // Execute git clone whole repo if there is no need to download only subdirectory
      query << GitHubConstants::GIT_CLONE << " " << repositoryAddress;
    }
    else
    {
      // Clone specified subdirectories that represent separate component
      fillGitCloneSubdirectoryQuery(query, directoryName, downloadPath, repositoryName, repositoryAddress);
    }

    ScExec exec{{query.str()}};
    return true;
  }

protected:
  static std::string GetDefaultBranch(std::string const & username, std::string const & repositoryName)
  {
    std::stringstream query;
    query << GitHubConstants::GITHUB_GET_DEFAULT_BRANCH_COMMAND_PREFIX << GitHubConstants::CURL_GET_BRANCH_COMMAND
          << username << SpecificationConstants::DIRECTORY_DELIMITER << repositoryName
          << GitHubConstants::GREP_DEFAULT_BRANCH_COMMAND;

    ScExec exec{{query.str()}};
    std::string const branches(std::istreambuf_iterator<char>(exec), {});
    size_t const actualBranchIndex = branches.find('\n');

    return branches.substr(0, actualBranchIndex);
  }

  static void fillGitCloneSubdirectoryQuery(
      std::stringstream & query,
      std::string const & directoryName,
      std::string const & downloadPath,
      std::string const & repositoryName,
      std::string const & repositoryAddress)
  {
    // Get all subdirectories of existing components from repo to add them to the sparse checkout
    std::stringstream existingComponentsName;
    existingComponentsName << directoryName << fillExistingComponents(downloadPath, repositoryName);

    std::string const directoryPathFromRepo = getDirectoryPathFromRepo(downloadPath, repositoryName);
    // Do not clone repository if it exists
    if (directoryPathFromRepo.empty())
    {
      fillGitCloneQuery(query, repositoryAddress);
      // The last argument is the repository address to clone
      query << " && ";
    }

    // Navigate to the component inner directory to execute commands here
    query << "cd " << repositoryName << " && ";

    // Sparse checkout to prepare current directory content
    query << GitHubConstants::GIT_SPARSE_CHECKOUT << " " << existingComponentsName.str() << " && ";

    // Git checkout to get current directory content
    query << GitHubConstants::GIT_CHECKOUT;
  }

  static void fillGitCloneQuery(std::stringstream & query, std::string const & repositoryAddress)
  {
    // Get a local copy of the remote repository
    query << GitHubConstants::GIT_CLONE << " ";

    // Create the repository only with .github, no copy all files now
    query << GitHubConstants::FLAG_NO_CHECKOUT << " ";

    // Get only the most recent commits and the current branch. Do not copy all history
    query << GitHubConstants::FLAG_DEPTH << "1"
          << " ";

    // Clone only metadata about files and directories of the repository
    query << GitHubConstants::FLAG_FILTER_TREE << "0"
          << " ";

    // The last argument is the repository address to clone
    query << repositoryAddress;
  }

  // Get all subdirectories of existing components from repo to add them to the sparse checkout
  static std::string fillExistingComponents(std::string const & downloadPath, std::string const & repositoryName)
  {
    std::string const componentPathFromRepo = getDirectoryPathFromRepo(downloadPath, repositoryName);
    std::stringstream existingComponentsName;
    if (!componentPathFromRepo.empty())
    {
      std::string existingComponentName;
      size_t const existingComponentNameStartIndex =
          downloadPath.size() + repositoryName.size() + 2;  // Two '/' symbols
      for (auto const & directory : std::filesystem::directory_iterator(componentPathFromRepo))
      {
        // Do not process .git directory
        existingComponentName = directory.path().string().substr(existingComponentNameStartIndex);
        if (existingComponentName[0] != '.')
        {
          existingComponentsName << " " << existingComponentName;
        }
      }
    }

    return existingComponentsName.str();
  }

  static std::string getDirectoryPathFromRepo(std::string const & path, std::string const & dirName)
  {
    std::stringstream directoryPath;
    directoryPath << path << SpecificationConstants::DIRECTORY_DELIMITER << dirName;
    if (std::filesystem::exists(directoryPath.str()))
    {
      return directoryPath.str();
    }

    return "";
  }
};
