/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "src/manager/commands/sc_component_manager_command.hpp"
#include "src/manager/commands/command_init/repos_parser/repos_parser.hpp"
#include "src/manager/commands/command_init/repos_downloader/repos_downloader.hpp"
#include "src/manager/commands/command_init/repos_downloader/repos_downloader_handler.hpp"

class ScComponentManagerCommandInit : public ScComponentManagerCommand
{
public:
  ScComponentManagerCommandInit(std::string reposPath, std::string specificationsPath)
    : m_reposPath(std::move(reposPath)), m_specificationsPath(std::move(specificationsPath))
  {
  }

  ExecutionResult Execute(ScMemoryContext * context, CommandParameters const & commandParameters) override
  {
    ExecutionResult executionResult;

    ReposParser parser;
    parser.Parse(m_reposPath);

    ReposDownloaderHandler downloaderHandler;
    std::vector<std::string> parsedComponents = parser.GetComponents();
    for (std::string const & componentPath : parsedComponents)
      downloaderHandler.Handle(componentPath, m_specificationsPath);

    return executionResult;
  }

private:
  std::string m_reposPath;

  std::string m_specificationsPath;
};
