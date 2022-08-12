/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc_component_manager_command.hpp"

class ScComponentManagerCommandInit : public ScComponentManagerCommand
{
public:
  ExecutionResult Execute(ScMemoryContext * context, CommandParameters const & commandParameters) override
  {
    ExecutionResult executionResult;
    std::fstream repos;
    repos.open(m_reposPath, std::ios::in);
    if (repos.is_open())
    {
      std::string line;
      while (std::getline(repos, line))
        executionResult.push_back(line);

      repos.close();
    }

    return executionResult;
  }
};
