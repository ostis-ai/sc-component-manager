/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "common/repository_url_parser.hpp"

void RepositoryUrlParser::Parse(std::string const & repositoryUrlAddress)
{
  m_urlAddress = repositoryUrlAddress;
  m_username = ParseUsername();
  m_repositoryName = ParseRepositoryName();
  m_branchName = ParseBranchName();
  m_directoryName = ParseDirectoryName();
}

// Get username from the url. For example "MksmOrlov" from https://github.com/MksmOrlov/ostis-kb-lib
std::string RepositoryUrlParser::ParseUsername()
{
  std::string username;
  size_t const usernameRepositoryNamePosition =
      m_urlAddress.find(GitHubConstants::GITHUB_PREFIX) + GitHubConstants::GITHUB_PREFIX.size();
  if (usernameRepositoryNamePosition == std::string::npos)
  {
    return username;
  }
  std::string const componentUsernameRepositoryName = m_urlAddress.substr(usernameRepositoryNamePosition);

  size_t const usernameRepositoryNameDelimiter =
      componentUsernameRepositoryName.find(SpecificationConstants::DIRECTORY_DELIMITER);
  if (usernameRepositoryNameDelimiter == std::string::npos)
  {
    return username;
  }
  username = componentUsernameRepositoryName.substr(0, usernameRepositoryNameDelimiter);
  return username;
}

// Get repository name from the url. For example "ostis-kb-lib" from https://github.com/MksmOrlov/ostis-kb-lib
std::string RepositoryUrlParser::ParseRepositoryName()
{
  std::string repositoryName;
  size_t const usernameStartPosition = m_urlAddress.find(m_username);
  if (usernameStartPosition == std::string::npos)
  {
    return repositoryName;
  }

  size_t const repositoryNameStartPosition =
      m_urlAddress.find(SpecificationConstants::DIRECTORY_DELIMITER, usernameStartPosition) + 1;
  if (repositoryNameStartPosition == std::string::npos || repositoryNameStartPosition == 0)
  {
    return repositoryName;
  }
  size_t const repositoryNameEndPosition =
      m_urlAddress.find(SpecificationConstants::DIRECTORY_DELIMITER, repositoryNameStartPosition + 1);
  m_repositoryUrl = m_urlAddress.substr(0, repositoryNameEndPosition);

  if (repositoryNameEndPosition == std::string::npos)
  {
    repositoryName = m_urlAddress.substr(repositoryNameStartPosition);
  }
  else
  {
    repositoryName =
        m_urlAddress.substr(repositoryNameStartPosition, repositoryNameEndPosition - repositoryNameStartPosition);
  }

  return repositoryName;
}

// Get branch name from the url. For example "main" from
// https://github.com/MksmOrlov/ostis-kb-lib/tree/main/part_methods_tools
std::string RepositoryUrlParser::ParseBranchName()
{
  std::string branchName;
  size_t const treeStartPosition = m_urlAddress.find(GitHubConstants::TREE);
  if (treeStartPosition == std::string::npos)
  {
    return branchName;
  }

  size_t const branchNameStartPosition =
      m_urlAddress.find(SpecificationConstants::DIRECTORY_DELIMITER, treeStartPosition) + 1;
  if (branchNameStartPosition == std::string::npos || branchNameStartPosition == 0)
  {
    return branchName;
  }

  size_t const branchNameEndPosition =
      m_urlAddress.find(SpecificationConstants::DIRECTORY_DELIMITER, branchNameStartPosition);
  if (branchNameEndPosition == std::string::npos || branchNameEndPosition == 0)
  {
    return branchName;
  }

  branchName = m_urlAddress.substr(branchNameStartPosition, branchNameEndPosition - branchNameStartPosition);
  return branchName;
}

// Get directory name from the url. For example "part_methods_tools" from
// https://github.com/MksmOrlov/ostis-kb-lib/tree/main/part_methods_tools
std::string RepositoryUrlParser::ParseDirectoryName()
{
  std::string directoryName;
  size_t const repositoryNameStartPosition = m_urlAddress.find(m_branchName);
  if (repositoryNameStartPosition == std::string::npos || repositoryNameStartPosition == 0)
  {
    return directoryName;
  }

  size_t const directoryNameStartPosition =
      m_urlAddress.find(SpecificationConstants::DIRECTORY_DELIMITER, repositoryNameStartPosition) + 1;
  if (directoryNameStartPosition == std::string::npos || directoryNameStartPosition == 0)
  {
    return directoryName;
  }

  directoryName = m_urlAddress.substr(directoryNameStartPosition);
  return directoryName;
}

std::string RepositoryUrlParser::GetUrlAddress()
{
  return m_urlAddress;
}

std::string RepositoryUrlParser::GetUsername()
{
  return m_username;
}

std::string RepositoryUrlParser::GetRepositoryName()
{
  return m_repositoryName;
}

std::string RepositoryUrlParser::GetDirectoryName()
{
  return m_directoryName;
}

std::string RepositoryUrlParser::GetRepositoryUrl()
{
  return m_repositoryUrl;
}

std::string RepositoryUrlParser::GetBranchName()
{
  return m_branchName;
}
