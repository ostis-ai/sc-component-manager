/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>
#include <vector>

class ReposParser
{
public:
  void Parse(std::string const & reposPath);

  std::vector<std::string> GetComponents();

  std::vector<std::string> GetRepositories();

protected:
  std::vector<std::string> m_components;

  std::vector<std::string> m_repositories;
};