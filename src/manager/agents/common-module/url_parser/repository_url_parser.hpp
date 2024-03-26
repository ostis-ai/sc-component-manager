/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>
#include "../constants/command_constants.hpp"

class RepositoryUrlParser
{
public:
  void Parse(std::string const & repositoryUrlAddress);

  std::string GetUrlAddress();
  std::string GetUsername();
  std::string GetRepositoryName();
  std::string GetDirectoryName();
  std::string GetRepositoryUrl();
  std::string GetBranchName();

protected:
  std::string m_urlAddress;
  std::string m_username;
  std::string m_repositoryName;
  std::string m_directoryName;
  std::string m_repositoryUrl;
  std::string m_branchName;

  std::string ParseUsername();
  std::string ParseRepositoryName();
  std::string ParseDirectoryName();
  std::string ParseBranchName();
};
